/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CACHE_H
#define ASI_CACHE_H

#include <asml/utils/error.h>
#include <ql/patterns/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

namespace ASI
{

    class ObjectInCache
    {
    public:
        virtual ~ObjectInCache();
    };

    template<typename T>
    class ObjectWrapper : public ObjectInCache
    {
    public:
        typedef boost::shared_ptr<const T> WrappedObject_t;

        ObjectWrapper(const WrappedObject_t & value) : m_value(value)
        {
        }

        const WrappedObject_t & value() const
        {
            return m_value;
        }

    private:
        const WrappedObject_t m_value;
    };

    class ObjectCache: public QuantLib::Singleton<ObjectCache>
    {
        friend class QuantLib::Singleton<ObjectCache>;

    private:
        ObjectCache();
        typedef boost::shared_ptr<const ObjectInCache> ObjectPtr_t;

    public:
        template<typename T>
        std::string store(const std::string & name, const boost::shared_ptr<const T> & value)
        {
            const ObjectPtr_t object(new ObjectWrapper<T>(value));
            const std::string key = storeObject(name, object);
            return key;
        }

        template<typename T>
        boost::shared_ptr<const T> get(const std::string & name) const
        {
            const ObjectPtr_t & object = getObject(name);
            const boost::shared_ptr<const ObjectWrapper<T> > objectTyped = boost::dynamic_pointer_cast<const ObjectWrapper<T> >(object);
            if (!objectTyped)
                THROW_EXCEPTION("Object " << name << " found, but of the wrong type");
            const boost::shared_ptr<const T> & value = objectTyped->value();
            return value;
        }

    private:
        static std::string encode(const std::string & name, const ObjectPtr_t & object);

        std::string storeObject(const std::string & name, const ObjectPtr_t & object);
        const ObjectPtr_t & getObject(const std::string & key) const;

        void removeOldObject(const std::string & name);
        std::string addNewObject(const std::string & name, const ObjectPtr_t & value);

        // real map, using unique identifiers
        std::map<std::string, ObjectPtr_t> m_objectCache;

        // map original name -> unique identifier
        // used to delete previous value when new one arrives
        std::map<std::string, std::string> m_nameKeyMap;
    };
}

#endif
