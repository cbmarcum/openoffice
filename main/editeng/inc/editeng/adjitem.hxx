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


#ifndef _SVX_ADJITEM_HXX
#define _SVX_ADJITEM_HXX

// include ---------------------------------------------------------------

#include <svl/eitem.hxx>
#include <editeng/svxenum.hxx>
#include <editeng/eeitem.hxx>
#include <editeng/editengdllapi.h>

class SvXMLUnitConverter;
namespace rtl
{
	class OUString;
}

// class SvxAdjustItem ---------------------------------------------------

/*
[Beschreibung]
Dieses Item beschreibt die Zeilenausrichtung.
*/
#define	ADJUST_LASTBLOCK_VERSION		((sal_uInt16)0x0001)

class EDITENG_DLLPUBLIC SvxAdjustItem : public SfxEnumItemInterface
{
	sal_Bool    bLeft      : 1;
	sal_Bool    bRight     : 1;
	sal_Bool    bCenter    : 1;
	sal_Bool    bBlock     : 1;

	// nur aktiv, wenn bBlock
	sal_Bool    bOneBlock : 1;
	sal_Bool    bLastCenter : 1;
	sal_Bool    bLastBlock : 1;

	friend SvStream& operator<<( SvStream&, SvxAdjustItem& ); //$ ostream
public:
	TYPEINFO();

    SvxAdjustItem( const SvxAdjust eAdjst /*= SVX_ADJUST_LEFT*/,
                   const sal_uInt16 nId );

	// "pure virtual Methoden" vom SfxPoolItem
	virtual int 			 operator==( const SfxPoolItem& ) const;

	virtual	sal_Bool        	 QueryValue( com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 ) const;
	virtual	sal_Bool			 PutValue( const com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 );

	virtual SfxItemPresentation GetPresentation( SfxItemPresentation ePres,
									SfxMapUnit eCoreMetric,
									SfxMapUnit ePresMetric,
                                    String &rText, const IntlWrapper * = 0 ) const;
	virtual sal_uInt16			 GetValueCount() const;
	virtual String			 GetValueTextByPos( sal_uInt16 nPos ) const;
	virtual sal_uInt16			 GetEnumValue() const;
	virtual void			 SetEnumValue( sal_uInt16 nNewVal );
	virtual SfxPoolItem*	 Clone( SfxItemPool *pPool = 0 ) const;
	virtual SfxPoolItem*	 Create(SvStream &, sal_uInt16) const;
	virtual SvStream&		 Store(SvStream &, sal_uInt16 nItemVersion ) const;
	virtual sal_uInt16			 GetVersion( sal_uInt16 nFileVersion ) const;

	inline void SetOneWord( const SvxAdjust eType )
	{
		bOneBlock  = eType == SVX_ADJUST_BLOCK;
	}

	inline void SetLastBlock( const SvxAdjust eType )
	{
		bLastBlock = eType == SVX_ADJUST_BLOCK;
		bLastCenter = eType == SVX_ADJUST_CENTER;
	}

	inline void SetAdjust( const SvxAdjust eType )
	{
		bLeft = eType == SVX_ADJUST_LEFT;
		bRight = eType == SVX_ADJUST_RIGHT;
		bCenter = eType == SVX_ADJUST_CENTER;
		bBlock = eType == SVX_ADJUST_BLOCK;
	}

	inline SvxAdjust GetLastBlock() const
	{
		SvxAdjust eRet = SVX_ADJUST_LEFT;

		if ( bLastBlock )
			eRet = SVX_ADJUST_BLOCK;
		else if( bLastCenter )
			eRet = SVX_ADJUST_CENTER;
		return eRet;
	}

	inline SvxAdjust GetOneWord() const
	{
		SvxAdjust eRet = SVX_ADJUST_LEFT;

		if ( bBlock && bOneBlock )
			eRet = SVX_ADJUST_BLOCK;
		return eRet;
	}

	inline SvxAdjust GetAdjust() const
	{
		SvxAdjust eRet = SVX_ADJUST_LEFT;

		if ( bRight )
			eRet = SVX_ADJUST_RIGHT;
		else if ( bCenter )
			eRet = SVX_ADJUST_CENTER;
		else if ( bBlock )
			eRet = SVX_ADJUST_BLOCK;
		return eRet;
	}
};

#endif

