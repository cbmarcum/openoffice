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



#ifndef __FRAMEWORK_CLASSES_PROTOCOLHANDLERCACHE_HXX_
#define __FRAMEWORK_CLASSES_PROTOCOLHANDLERCACHE_HXX_

//_________________________________________________________________________________________________________________
//	my own includes
//_________________________________________________________________________________________________________________

#include <general.h>
#include <stdtypes.h>
#include <macros/debug.hxx>

//_________________________________________________________________________________________________________________
//	interface includes
//_________________________________________________________________________________________________________________
#include <com/sun/star/util/URL.hpp>

//_________________________________________________________________________________________________________________
//	other includes
//_________________________________________________________________________________________________________________

#include <unotools/configitem.hxx>
#include <rtl/ustring.hxx>
#include <fwidllapi.h>

//_________________________________________________________________________________________________________________
//	namespace
//_________________________________________________________________________________________________________________

namespace framework{

//_________________________________________________________________________________________________________________
//	exported const
//_________________________________________________________________________________________________________________

#define PACKAGENAME_PROTOCOLHANDLER                 DECLARE_ASCII("Office.ProtocolHandler"                          )   /// name of our configuration package

#define CFG_PATH_SEPERATOR                          DECLARE_ASCII("/"                                               )   /// seperator for configuration paths
#define CFG_ENCODING_OPEN                           DECLARE_ASCII("[\'"                                             )   /// used to start encoding of set names
#define CFG_ENCODING_CLOSE                          DECLARE_ASCII("\']"                                             )   /// used to finish encoding of set names

#define SETNAME_HANDLER                             DECLARE_ASCII("HandlerSet"                                      )   /// name of configuration set inside package
#define PROPERTY_PROTOCOLS                          DECLARE_ASCII("Protocols"                                       )   /// properties of a protocol handler

//_________________________________________________________________________________________________________________

/**
    Programmer can register his own services to handle different protocols.
    Don't forget: It doesn't mean "handling of documents" ... these services could handle protocols ...
    e.g. "mailto:", "file://", ".java:"
    This struct holds the information about one such registered protocol handler.
    A list of handler objects is defined as ProtocolHandlerHash. see below
*/
struct FWI_DLLPUBLIC ProtocolHandler
{
    /* member */
    public:

        /// the uno implementation name of this handler
        ::rtl::OUString m_sUNOName;
        /// list of URL pattern which defines the protocols which this handler is registered for
        OUStringList m_lProtocols;
};

//_________________________________________________________________________________________________________________

/**
    This hash use registered pattern of all protocol handlers as keys and provide her
    uno implementation names as value. Overloading of the index operator makes it possible
    to search for a key by using a full qualified URL on list of all possible pattern keys.
*/
class FWI_DLLPUBLIC PatternHash : public BaseHash< ::rtl::OUString >
{
    /* interface */
	public:

        PatternHash::iterator findPatternKey( const ::rtl::OUString& sURL );
};

//_________________________________________________________________________________________________________________

/**
    This hash holds protocol handler structs by her names.
*/
typedef BaseHash< ProtocolHandler > HandlerHash;

//_________________________________________________________________________________________________________________

/**
    @short          this hash makes it easy to find a protocol handler by using his uno implementation name.
    @descr          It holds two lists of informations:
                        - first holds all handler by her uno implementation names and
                          can be used to get her other properties
                        - another one maps her registered pattern to her uno names to
                          perform search on such data
                    But this lists a static for all instances of this class. So it's possible to
                    create new objects without opening configuration twice and free memory automatically
                    if last object will gone.

    @attention      We implement a singleton concept - so we doesn't need any mutex member here.
                    Because to safe access on static member we must use a static global lock
                    here too.

	@devstatus		ready to use
    @threadsafe     yes

    @modified       30.04.2002 11:19, as96863
*/

class HandlerCFGAccess;
class FWI_DLLPUBLIC HandlerCache
{
    /* member */
    private:

        /// list of all registered handler registered by her uno implementation names
        static HandlerHash* m_pHandler;
        /// maps URL pattern to handler names
        static PatternHash* m_pPattern;
        /// informs about config updates
        static HandlerCFGAccess* m_pConfig;
        /// ref count to construct/destruct internal member lists on demand by using singleton mechanism
        static sal_Int32 m_nRefCount;

    /* interface */
    public:

                 HandlerCache();
        virtual ~HandlerCache();

        sal_Bool search( const ::rtl::OUString& sURL, ProtocolHandler* pReturn ) const;
        sal_Bool search( const css::util::URL&  aURL, ProtocolHandler* pReturn ) const;
        sal_Bool exists( const ::rtl::OUString& sURL ) const;
        
        void takeOver(HandlerHash* pHandler, PatternHash* pPattern);
};

//_________________________________________________________________________________________________________________

/**
    @short          implements configuration access for handler configuration
    @descr          We use the ConfigItem mechanism to read/write values from/to configuration.
                    We set a data container pointer for filling or reading ... this class use it temp.
                    After successfuly calling of read(), we can use filled container directly or merge it with an existing one.
                    After successfuly calling of write() all values of given data container are flushed to our configuration -
                    but current implementation doesn't support writeing really.

    @base           ::utl::ConfigItem
                    base mechanism for configuration access

	@devstatus		ready to use
    @threadsafe     no

    @modified       30.04.2002 09:58, as96863
*/
class FWI_DLLPUBLIC HandlerCFGAccess : public ::utl::ConfigItem
{
    private:
        HandlerCache* m_pCache;

    /* interface */
    public:
                 HandlerCFGAccess( const ::rtl::OUString& sPackage  );
        void     read            (       HandlerHash**    ppHandler ,
                                         PatternHash**    ppPattern );

        void setCache(HandlerCache* pCache) {m_pCache = pCache;};
        virtual void Notify(const css::uno::Sequence< rtl::OUString >& lPropertyNames);
		virtual void Commit();
};

} // namespace framework

#endif // #ifndef __FRAMEWORK_CLASSES_PROTOCOLHANDLERCACHE_HXX_
