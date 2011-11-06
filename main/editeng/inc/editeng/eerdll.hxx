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



#ifndef _EERDLL_HXX
#define _EERDLL_HXX

class GlobalEditData;

#include <tools/resid.hxx>
#include <tools/shl.hxx>
#include <editeng/editengdllapi.h>

class EDITENG_DLLPUBLIC EditResId: public ResId
{
public:
	EditResId( sal_uInt16 nId );
};

class EditDLL
{
	ResMgr*			pResMgr;
	GlobalEditData*	pGlobalData;

public:
					EditDLL();
					~EditDLL();

	ResMgr*			GetResMgr() const 		{ return pResMgr; }
	GlobalEditData*	GetGlobalData() const	{ return pGlobalData; }
	static EditDLL* Get();
};

#define EE_DLL() EditDLL::Get()

#define EE_RESSTR(x) String( EditResId(x) )

#endif //_EERDLL_HXX
