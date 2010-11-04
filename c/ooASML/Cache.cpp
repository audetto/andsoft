/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "Cache.h"
#include <asml/utils/utils.h>
#include <sstream>

using namespace std;

namespace ASI
{
    ObjectInCache::~ObjectInCache()
    {
    }

    ObjectCache::ObjectCache()
    {
    }

    string ObjectCache::storeObject(const string & name, const ObjectPtr_t & object)
    {
        removeOldObject(name);
        const string & key = addNewObject(name, object);
        return key;
    }

    void ObjectCache::removeOldObject(const string & name)
    {
        const std::map<std::string, std::string>::iterator it = m_nameKeyMap.find(name);
        if (it != m_nameKeyMap.end())
        {
            const std::string & key = it->second;
            m_objectCache.erase(key);
            m_nameKeyMap.erase(it);
        }
    }

    string ObjectCache::addNewObject(const string & name, const ObjectPtr_t & object)
    {
        const string key = encode(name, object);
        m_nameKeyMap[name] = key;
        m_objectCache[key] = object;
        return key;
    }

    const ObjectCache::ObjectPtr_t & ObjectCache::getObject(const std::string & key) const
    {
        const ObjectPtr_t & object = findInMap(m_objectCache, key);
        return object;
    }
  
    string ObjectCache::encode(const std::string & name, const ObjectPtr_t & object)
    {
        ostringstream ss;
        ss << "< ";
        ss << name;
        ss << " @ ";
        ss << object.get();
        ss << " >";

        string key = ss.str();

        return key;
    }
   
}
