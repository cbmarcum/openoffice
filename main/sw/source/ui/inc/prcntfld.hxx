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


#ifndef _PRCNTFLD_HXX
#define _PRCNTFLD_HXX

#ifndef _FIELD_HXX //autogen
#include <vcl/field.hxx>
#endif
#include "swdllapi.h"

class SW_DLLPUBLIC PercentField : public MetricField
{
    sal_Int64   nRefValue;      // 100%-Wert fuer Umrechnung (in Twips)
    sal_Int64   nOldMax;
    sal_Int64   nOldMin;
    sal_Int64   nOldSpinSize;
    sal_Int64   nOldBaseValue;
    sal_Int64   nLastPercent;
    sal_Int64   nLastValue;
	sal_uInt16		nOldDigits;
	FieldUnit	eOldUnit;
    sal_Bool    bLockAutoCalculation; //prevent recalcution of percent values when the
                                        //reference value is changed

    SW_DLLPRIVATE sal_Int64      ImpPower10(sal_uInt16 n);

    using MetricField::SetValue;
    using MetricField::GetValue;
    using MetricFormatter::SetUserValue;
    using MetricFormatter::SetBaseValue;
    using MetricFormatter::SetMax;
    using MetricFormatter::SetMin;
    using NumericFormatter::IsValueModified;

public:

    virtual void  SetValue(sal_Int64 nNewValue, FieldUnit eInUnit = FUNIT_NONE);

	PercentField( Window* pWin, const ResId& rResId );

    void        SetPrcntValue(sal_Int64 nNewValue, FieldUnit eInUnit = FUNIT_NONE);

    void        SetUserValue(sal_Int64 nNewValue, FieldUnit eInUnit = FUNIT_NONE);

    void        SetBaseValue(sal_Int64 nNewValue, FieldUnit eInUnit = FUNIT_NONE);

    sal_Int64        GetValue(FieldUnit eOutUnit = FUNIT_NONE);

	sal_Bool		IsValueModified();

	//using NumericFormatter::SetMax;
    void        SetMax(sal_Int64 nNewMax, FieldUnit eInUnit = FUNIT_NONE);

	//using NumericFormatter::SetMin;
    void        SetMin(sal_Int64 nNewMin, FieldUnit eInUnit = FUNIT_NONE);

    sal_Int64        NormalizePercent(sal_Int64 nValue);
    sal_Int64        DenormalizePercent(sal_Int64 nValue);

    void        SetRefValue(sal_Int64 nValue);
    inline sal_Int64 GetRefValue() const { return nRefValue; }
    sal_Int64   GetRealValue(FieldUnit eOutUnit = FUNIT_NONE);

    sal_Int64   Convert(sal_Int64 nValue, FieldUnit eInUnit, FieldUnit eOutUnit);

	void		ShowPercent(sal_Bool bPercent);

	sal_uInt16		GetOldDigits() const {return nOldDigits;}

    void        LockAutoCalculation(sal_Bool bLock) {bLockAutoCalculation = bLock;}
    sal_Bool    IsAutoCalculationLocked()const {return bLockAutoCalculation;}
};

#endif // _PRCNTFLD_HXX

