/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/JSPayoff.h>
#include <asml/payoff/V8Utils.h>
#include <asml/utils/error.h>
#include <v8.h>
#include <fcntl.h>
#include <boost/foreach.hpp>

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
        
        try
        {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            rewind(file);
        
            vector<char> chars(size + 1);
            chars[size] = '\0';
            for (int i = 0; i < size;)
            {
                int read = fread(&chars[i], 1, size - i, file);
                i += read;
                if (read == 0)
                    THROW_EXCEPTION("V8: While reading " << name << ", stopped after " << i << " out of " << size);
            }
            fclose(file);
            Handle<v8::String> result = String::New(&chars[0], size);
            return result;
        }
        catch (...)
        {
            fclose(file);
            throw;
        }
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

    Handle<Value> TS_DF_Method(const Arguments& args)
    {
        Local<Object> self = args.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        typedef std::vector<QuantLib::Handle<QuantLib::YieldTermStructure> > VectorTS_t;

        const VectorTS_t * forwardTermStructures = (const VectorTS_t *)ptr;

        if (args.Length() != 2)
            return ThrowException(String::New("Invalid number of arguments"));
        
        const uint32_t datePos = args[0]->Uint32Value();
        const uint32_t dateMat = args[1]->Uint32Value();
        
        try
        {
            QuantLib::Date expiry(dateMat);
            if (datePos >= forwardTermStructures->size())
                return ThrowException(String::New("Invalid date pos"));
            
            const double df = (*forwardTermStructures)[datePos]->discount(expiry);
            return Number::New(df);
        }
        catch (const exception & e)
        {
            return ThrowException(String::New(e.what()));
        }
    }

    void loadFile(const TryCatch & tryCatch, const string & filename)
    {
        // Create a string containing the JavaScript source code.
        Handle<String> source = ReadFile(filename);
        
        // Compile the source code.
        Handle<Script> script = Script::Compile(source, String::New(filename.c_str()));
        throwException(tryCatch);

        Handle<Value> result = script->Run();
        throwException(tryCatch);
    }
}

namespace ASI
{

    struct JSPayoff::PImpl
    {
        Persistent<Context>  context;
        Persistent<Function> dates;
        Persistent<Function> payoff;
        Persistent<Function> yieldCurve;

        ~PImpl();
    };

    JSPayoff::PImpl::~PImpl()
    {
        yieldCurve.Dispose();
        payoff.Dispose();
        dates.Dispose();
        context.Dispose();
    }
    
    JSPayoff::JSPayoff(const vector<string>   & includes,
                       const string           & filename)
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

        BOOST_FOREACH(const string & inc, includes)
        {
            if (!inc.empty())
                loadFile(tryCatch, inc);
        }
      
        loadFile(tryCatch, filename);

        // The script compiled and ran correctly.  Now we fetch out the
        // Payoff function from the global object.
        Handle<Function> payoff_fun = extractFunction(m_pimpl->context, "payoff");
        Handle<Function> dates_fun = extractFunction(m_pimpl->context, "dates");

        // Store the function in a Persistent handle, since we also want
        // that to remain after this call returns
        m_pimpl->payoff = Persistent<Function>::New(payoff_fun);
        m_pimpl->dates  = Persistent<Function>::New(dates_fun);

        Handle<FunctionTemplate> ts_templ = FunctionTemplate::New();
        ts_templ->SetClassName(String::New("TS"));
        Handle<ObjectTemplate> ts_proto = ts_templ->PrototypeTemplate();
        ts_proto->Set("df", FunctionTemplate::New(TS_DF_Method));
        Handle<ObjectTemplate> ts_inst = ts_templ->InstanceTemplate();
        ts_inst->SetInternalFieldCount(1);

        Handle<Function> yieldCurve = ts_templ->GetFunction();

        m_pimpl->yieldCurve = Persistent<Function>::New(yieldCurve);
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

        Handle<Object> v8Path = convertMatrixToV8(path);

        Local<Object> yc = m_pimpl->yieldCurve->NewInstance();
        yc->SetInternalField(0, External::New((void *)&forwardTermStructures));

        const int argc = 2;
        Handle<Value> argv[argc] = { v8Path, yc };

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
