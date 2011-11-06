/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_ucb.hxx"
#include "cachemapobject1.hxx"
#include "osl/diagnose.h"
#include "osl/interlck.h"
#include "osl/mutex.hxx"
#include "rtl/ref.hxx"
#include "rtl/ustring.hxx"

#ifndef INCLUDED_MEMORY
#include <memory>
#define INCLUDED_MEMORY
#endif

using ucb::cachemap::Object1;
using ucb::cachemap::ObjectContainer1;

inline
Object1::Object1(rtl::Reference< ObjectContainer1 > const & rContainer):
    m_xContainer(rContainer),
    m_nRefCount(0)
{
    OSL_ASSERT(m_xContainer.is());
}

inline Object1::~Object1() SAL_THROW(())
{}

void ObjectContainer1::releaseElement(Object1 * pElement) SAL_THROW(())
{
    OSL_ASSERT(pElement);
    bool bDelete = false;
    {
        osl::MutexGuard aGuard(m_aMutex);
        if (osl_decrementInterlockedCount(&pElement->m_nRefCount) == 0)
        {
            m_aMap.erase(pElement->m_aContainerIt);
            bDelete = true;
        }
    }
    if (bDelete)
        delete pElement;
}

ObjectContainer1::ObjectContainer1()
{}

ObjectContainer1::~ObjectContainer1() SAL_THROW(())
{}

rtl::Reference< Object1 > ObjectContainer1::get(rtl::OUString const & rKey)
{
    osl::MutexGuard aGuard(m_aMutex);
    Map::iterator aIt(m_aMap.find(rKey));
    if (aIt == m_aMap.end())
    {
        std::auto_ptr< Object1 > xElement(new Object1(this));
        aIt = m_aMap.insert(Map::value_type(rKey, xElement.get())).first;
        aIt->second->m_aContainerIt = aIt;
        xElement.release();
    }
    return aIt->second;
}
