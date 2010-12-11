/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/JSPayoff.h>
#include <asml/payoff/V8Utils.h>
#include <asml/utils/error.h>
#include <v8.h>
#include <fcntl.h>

using namespace v8;
using namespace std;
using namespace ASI;

namespace
{
    // Reads a file into a v8 string.
    Handle<String> ReadFile(const string & name)
    {
        FILE* file = fopen(name.c_str(), "rb");
        if (file == NULL)
            THROW_EXCEPTION("V8: Cannot read file: " << name);
        
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        
        char* chars = new char[size + 1];
        chars[size] = '\0';
        for (int i = 0; i < size;)
        {
            int read = fread(&chars[i], 1, size - i, file);
            i += read;
            if (read == 0)
                THROW_EXCEPTION("V8: While reading " << name << ", stopped after " << i << " out of " << size);
        }
        fclose(file);
        Handle<v8::String> result = String::New(chars, size);
        delete[] chars;
        return result;
    }

    void throwException(const TryCatch & tryCatch)
    {
        if (!tryCatch.HasCaught())
            return;
        String::Utf8Value stack_trace(tryCatch.StackTrace());

        if (stack_trace.length() > 0)
        {
            const char* stack_trace_string = *stack_trace;
            THROW_EXCEPTION(stack_trace_string);
        }
        else
        {
            Handle<Value> exception = tryCatch.Exception();
            String::AsciiValue exception_str(exception);
            THROW_EXCEPTION(*exception_str);
        }
    }

    Handle<Array> convertMatrixToV8(const QuantLib::Matrix & mat)
    {
        const size_t rows = mat.rows();
        const size_t cols = mat.columns();

        Handle<Array> res = Array::New(rows);

        for (size_t i = 0; i < rows; ++i)
        {
            Handle<Array> thisRow = Array::New(cols);
            for (size_t j = 0; j < cols; ++j)
            {
                thisRow->Set(j, Number::New(mat[i][j]));
            }
            res->Set(i, thisRow);
        }

        return res;
    }

    void convertIfPresent(Handle<Value> result, QuantLib::Array & payments)
    {
        if (!result->IsArray())
            THROW_EXCEPTION("V8: Not an array");

        Handle<Array> arr = Handle<Array>::Cast(result);
        const size_t numberOfTimes = payments.size();
        const size_t resultSize = arr->Length();

        if (resultSize != numberOfTimes)
            THROW_EXCEPTION("V8: Wrong Number of Times: " << resultSize << " != " << numberOfTimes);

        for (size_t i = 0; i < numberOfTimes; ++i)
        {
            Handle<Value> val = arr->Get(i);
            if (val->IsNumber())
            {
                const double value = val->NumberValue();
                payments[i] = value;
            }
        }

    }

    Handle<Function> extractFunction(Handle<Context> context, const string & name)
    {
        Handle<String> payoff_name = String::New(name.c_str());
        Handle<Value> payoff_val = context->Global()->Get(payoff_name);

        // If there is no Payoff function, or if it is not a function,
        // bail out
        if (!payoff_val->IsFunction()) 
            THROW_EXCEPTION("V8: " << name << " is not a function");

        // It is a function; cast it to a Function
        Handle<Function> payoff_fun = Handle<Function>::Cast(payoff_val);
        
        return payoff_fun;
    }    

}

namespace ASI
{

    struct JSPayoff::PImpl
    {
        Persistent<Context>  context;
        Persistent<Function> dates;
        Persistent<Function> payoff;

        ~PImpl();
    };

    JSPayoff::PImpl::~PImpl()
    {
        payoff.Dispose();
        dates.Dispose();
        context.Dispose();
    }
    
    JSPayoff::JSPayoff(const string & filename)
    {
        m_pimpl.reset(new PImpl);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope;
        
        // Create a new context.
        m_pimpl->context = Context::New();
        
        // Enter the created context for compiling and
        // running the hello world script. 
        Context::Scope context_scope(m_pimpl->context);

        TryCatch tryCatch;
      
        // Create a string containing the JavaScript source code.
        Handle<String> source = ReadFile(filename);
        
        // Compile the source code.
        Handle<Script> script = Script::Compile(source, String::New(filename.c_str()));
        throwException(tryCatch);

        Handle<Value> result = script->Run();
        throwException(tryCatch);

        // The script compiled and ran correctly.  Now we fetch out the
        // Payoff function from the global object.
        Handle<Function> payoff_fun = extractFunction(m_pimpl->context, "payoff");
        Handle<Function> dates_fun = extractFunction(m_pimpl->context, "dates");

        // Store the function in a Persistent handle, since we also want
        // that to remain after this call returns
        m_pimpl->payoff = Persistent<Function>::New(payoff_fun);
        m_pimpl->dates  = Persistent<Function>::New(dates_fun);
    }

    std::string JSPayoff::name() const
    {
        return "JSPayoff";
    }
    
    std::string JSPayoff::description() const
    {
        return name();
    }
    
    QuantLib::Size JSPayoff::basisSystemDimension() const
    {
        return 0;
    }
    
    void JSPayoff::value(const QuantLib::Matrix                                   & path, 
                         const std::vector<QuantLib::Handle<QuantLib::YieldTermStructure> > & forwardTermStructures,
                         QuantLib::Array                                          & payments, 
                         QuantLib::Array                                          & exercises, 
                         std::vector<QuantLib::Array>                             & states) const
    {
        // Create a stack-allocated handle scope.
        HandleScope handle_scope;

        Context::Scope context_scope(m_pimpl->context);

        TryCatch tryCatch;

        Handle<Object> arg = convertMatrixToV8(path);

        const int argc = 1;
        Handle<Value> argv[argc] = { arg };

        // Run the script to get the result.
        Handle<Value> result = m_pimpl->payoff->Call(m_pimpl->context->Global(), argc, argv); 
        throwException(tryCatch);

        convertIfPresent(result, payments);
    }

    vector<QuantLib::Date> JSPayoff::fixingDates() const
    {
        // Create a stack-allocated handle scope.
        HandleScope handle_scope;

        Context::Scope context_scope(m_pimpl->context);

        TryCatch tryCatch;

        // Run the script to get the result.
        Handle<Value> result = m_pimpl->dates->Call(m_pimpl->context->Global(), 0, NULL); 
        throwException(tryCatch);

        vector<QuantLib::Date> dates;
        v8ConvertIn(result, dates);

        return dates;
    }
}
