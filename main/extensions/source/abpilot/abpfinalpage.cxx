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
#include "precompiled_extensions.hxx"
#include "abpfinalpage.hxx"
#include "addresssettings.hxx"
#include "abspilot.hxx"
#include <tools/debug.hxx>
#include <tools/urlobj.hxx>
#include <unotools/ucbhelper.hxx>
#include <sfx2/filedlghelper.hxx>
#include <unotools/pathoptions.hxx>
#ifndef SVTOOLS_FILENOTATION_HXX_
#include <svl/filenotation.hxx>
#endif
#include <sfx2/docfilt.hxx>
#include <vcl/msgbox.hxx>
#include <comphelper/componentcontext.hxx>
#include <com/sun/star/ui/dialogs/TemplateDescription.hpp>

//.........................................................................
namespace abp
{
//.........................................................................
	using namespace ::svt;
	using namespace ::utl;

	const SfxFilter* lcl_getBaseFilter()
	{
		static const String s_sDatabaseType = String::CreateFromAscii("StarOffice XML (Base)");
		const SfxFilter* pFilter = SfxFilter::GetFilterByName( s_sDatabaseType);
		OSL_ENSURE(pFilter,"Filter: StarOffice XML (Base) could not be found!");
		return pFilter;
	}
	//=====================================================================
	//= FinalPage
	//=====================================================================
	//---------------------------------------------------------------------
	FinalPage::FinalPage( OAddessBookSourcePilot* _pParent )
		:AddressBookSourcePage(_pParent, ModuleRes(RID_PAGE_FINAL))
		,m_aExplanation			( this, ModuleRes( FT_FINISH_EXPL ) )
		,m_aLocationLabel		( this, ModuleRes( FT_LOCATION ) )
		,m_aLocation			( this, ModuleRes( CBB_LOCATION ) )
		,m_aBrowse				( this, ModuleRes( PB_BROWSE ) )
		,m_aRegisterName		( this, ModuleRes( CB_REGISTER_DS ) )
		,m_aNameLabel			( this, ModuleRes( FT_NAME_EXPL ) )
		,m_aName				( this, ModuleRes( ET_DATASOURCENAME ) )
		,m_aDuplicateNameError	( this, ModuleRes( FT_DUPLICATENAME ) )
        ,m_aLocationController( ::comphelper::ComponentContext( _pParent->getORB() ), m_aLocation, m_aBrowse )
	{
		FreeResource();

		m_aName.SetModifyHdl( LINK(this, FinalPage, OnNameModified) );
		m_aLocation.SetModifyHdl( LINK(this, FinalPage, OnNameModified) );
		m_aRegisterName.SetClickHdl( LINK( this, FinalPage, OnRegister ) );
		m_aRegisterName.Check(sal_True);
	}

	//---------------------------------------------------------------------
	sal_Bool FinalPage::isValidName() const
	{
		::rtl::OUString sCurrentName(m_aName.GetText());

		if (0 == sCurrentName.getLength())
			// the name must not be empty
			return sal_False;

		if ( m_aInvalidDataSourceNames.find( sCurrentName ) != m_aInvalidDataSourceNames.end() )
			// there already is a data source with this name
			return sal_False;

		return sal_True;
	}

	//---------------------------------------------------------------------
	void FinalPage::setFields()
	{
		AddressSettings& rSettings = getSettings();

		INetURLObject aURL( rSettings.sDataSourceName );
		if( aURL.GetProtocol() == INET_PROT_NOT_VALID )
		{
			String sPath = SvtPathOptions().GetWorkPath();
			sPath += '/';
			sPath += String(rSettings.sDataSourceName);

			const SfxFilter* pFilter = lcl_getBaseFilter();
			if ( pFilter )
			{
				String sExt = pFilter->GetDefaultExtension();
				sPath += sExt.GetToken(1,'*');
			}

			aURL.SetURL(sPath);
		}
		OSL_ENSURE( aURL.GetProtocol() != INET_PROT_NOT_VALID ,"No valid file name!");
		rSettings.sDataSourceName = aURL.GetMainURL( INetURLObject::NO_DECODE );
        m_aLocationController.setURL( rSettings.sDataSourceName );
		String sName = aURL.getName( );
		xub_StrLen nPos = sName.Search(String(aURL.GetExtension()));
		if ( nPos != STRING_NOTFOUND )
		{
			sName.Erase(nPos-1,4);
		}
		m_aName.SetText(sName);

		OnRegister(&m_aRegisterName);
	}

	//---------------------------------------------------------------------
	void FinalPage::initializePage()
	{
		AddressBookSourcePage::initializePage();

		setFields();
	}

	//---------------------------------------------------------------------
    sal_Bool FinalPage::commitPage( ::svt::WizardTypes::CommitPageReason _eReason )
	{
		if (!AddressBookSourcePage::commitPage(_eReason))
			return sal_False;

        if  (   ( ::svt::WizardTypes::eTravelBackward != _eReason )
            &&  ( !m_aLocationController.prepareCommit() )
            )
            return sal_False;

		AddressSettings& rSettings = getSettings();
		rSettings.sDataSourceName = m_aLocationController.getURL();
		rSettings.bRegisterDataSource = m_aRegisterName.IsChecked();
		if ( rSettings.bRegisterDataSource )
			rSettings.sRegisteredDataSourceName = m_aName.GetText();

		return sal_True;
	}

	//---------------------------------------------------------------------
	void FinalPage::ActivatePage()
	{
		AddressBookSourcePage::ActivatePage();

		// get the names of all data sources
		ODataSourceContext aContext( getORB() );
		aContext.getDataSourceNames( m_aInvalidDataSourceNames );

		// give the name edit the focus
		m_aLocation.GrabFocus();

		// default the finish button
		getDialog()->defaultButton( WZB_FINISH );
	}

	//---------------------------------------------------------------------
	void FinalPage::DeactivatePage()
	{
		AddressBookSourcePage::DeactivatePage();

		// default the "next" button, again
		getDialog()->defaultButton( WZB_NEXT );
		// disable the finish button
		getDialog()->enableButtons( WZB_FINISH, sal_False );
	}

	//---------------------------------------------------------------------
	bool FinalPage::canAdvance() const
	{
		return false;
	}

	//---------------------------------------------------------------------
	void FinalPage::implCheckName()
	{
		sal_Bool bValidName = isValidName();
		sal_Bool bEmptyName = 0 == m_aName.GetText().Len();
		sal_Bool bEmptyLocation = 0 == m_aLocation.GetText().Len();

		// enable or disable the finish button
		getDialog()->enableButtons( WZB_FINISH, !bEmptyLocation && (!m_aRegisterName.IsChecked() || bValidName) );

		// show the error message for an invalid name
		m_aDuplicateNameError.Show( !bValidName && !bEmptyName );
	}

	//---------------------------------------------------------------------
	IMPL_LINK( FinalPage, OnNameModified, Edit*, /**/ )
	{
		implCheckName();
		return 0L;
	}

    // -----------------------------------------------------------------------------
	IMPL_LINK( FinalPage, OnRegister, CheckBox*, EMPTYARG )
	{
		sal_Bool bEnable = m_aRegisterName.IsChecked();
		m_aNameLabel.Enable(bEnable);
		m_aName.Enable(bEnable);
		implCheckName();
		return 0L;
	}
//.........................................................................
}	// namespace abp
//.........................................................................

