#include "stdafx.h"
#include "Properties/BackglassVisualsProperty.h"
#include <WindowsX.h>

BackglassVisualsProperty::BackglassVisualsProperty(VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPBACKGLASS_VISUALS, pvsel)
{
}

void BackglassVisualsProperty::UpdateVisuals()
{
    CComObject<PinTable> * const table = g_pvp->GetActiveTable();
    PropertyDialog::SetCheckboxState(m_hApplyNightDayCheck, table->m_ImageBackdropNightDay);
    PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_dtImageCombo, table->m_BG_szImage[0]);
    PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_fsImageCombo, table->m_BG_szImage[1]);
    PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_fssImageCombo, table->m_BG_szImage[2]);
    PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_colorGradingCombo, table->m_szImageColorGrade);
    PropertyDialog::SetCheckboxState(m_hEnableDecal, table->m_renderDecals);
    PropertyDialog::SetCheckboxState(m_hEnableEMReelCheck, table->m_renderEMReels);
    PropertyDialog::SetCheckboxState(m_hOverwriteGlobalStereoSettingsCheck, table->m_overwriteGlobalStereo3D);
    PropertyDialog::SetFloatTextbox(m_3dStereoOffsetEdit, table->Get3DOffset());
    PropertyDialog::SetFloatTextbox(m_3dStereoSeparationEdit, table->GetMaxSeparation());
    PropertyDialog::SetFloatTextbox(m_3dSteroZPDEdit, table->GetZPD());
    m_colorButton1.SetColor(table->m_colorbackdrop);
}

void BackglassVisualsProperty::UpdateProperties(const int dispid)
{
    CComObject<PinTable> * const table = g_pvp->GetActiveTable();
    switch (dispid)
    {
        case IDC_BG_NIGHT_DAY:
            CHECK_UPDATE_ITEM(table->m_ImageBackdropNightDay, PropertyDialog::GetCheckboxState(m_hApplyNightDayCheck), table);
            break;
        case DISPID_Image2:
            CHECK_UPDATE_COMBO_TEXT(table->m_BG_szImage[0], m_dtImageCombo, table);
            break;
        case DISPID_Image6:
            CHECK_UPDATE_COMBO_TEXT(table->m_BG_szImage[1], m_fsImageCombo, table);
            break;
        case DISPID_Image8:
            CHECK_UPDATE_COMBO_TEXT(table->m_BG_szImage[2], m_fssImageCombo, table);
            break;
        case 1509:
            CHECK_UPDATE_COMBO_TEXT(table->m_szImageColorGrade, m_colorGradingCombo, table);
            break;
        case IDC_ENABLE_EMREEL_CHECK:
            CHECK_UPDATE_ITEM(table->m_renderEMReels, PropertyDialog::GetCheckboxState(m_hEnableEMReelCheck), table);
            break;
        case IDC_ENABLE_DECAL_CHECK:
            CHECK_UPDATE_ITEM(table->m_renderDecals, PropertyDialog::GetCheckboxState(m_hEnableDecal), table);
            break;
        case IDC_GLOBAL_3DSTEREO:
            CHECK_UPDATE_ITEM(table->m_overwriteGlobalStereo3D, PropertyDialog::GetCheckboxState(m_hOverwriteGlobalStereoSettingsCheck), table);
            break;
        case IDC_3D_STEREO_OFFSET_EDIT:
            CHECK_UPDATE_VALUE_SETTER(table->Set3DOffset, table->Get3DOffset, PropertyDialog::GetFloatTextbox, m_3dStereoOffsetEdit, table);
            break;
        case IDC_3D_STEREO_SEPARATION_EDIT:
            CHECK_UPDATE_VALUE_SETTER(table->SetMaxSeparation, table->GetMaxSeparation, PropertyDialog::GetFloatTextbox, m_3dStereoSeparationEdit, table);
            break;
        case IDC_3D_STEREO_ZPD_EDIT:
            CHECK_UPDATE_VALUE_SETTER(table->SetZPD, table->GetZPD, PropertyDialog::GetFloatTextbox, m_3dSteroZPDEdit, table);
            break;
        case IDC_COLOR_BUTTON1:
        {
            CHOOSECOLOR cc = m_colorDialog.GetParameters();
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            m_colorDialog.SetParameters(cc);
            m_colorDialog.SetColor(table->m_colorbackdrop);
            if (m_colorDialog.DoModal(GetHwnd()) == IDOK)
            {
                table->m_colorbackdrop = m_colorDialog.GetColor();
                m_colorButton1.SetColor(table->m_colorbackdrop);
            }
            break;
        }
        default:
            break;
    }
    UpdateVisuals();
}

BOOL BackglassVisualsProperty::OnInitDialog()
{
    m_hApplyNightDayCheck = ::GetDlgItem(GetHwnd(), IDC_BG_NIGHT_DAY);
    AttachItem(DISPID_Image2, m_dtImageCombo);
    AttachItem(DISPID_Image6, m_fsImageCombo);
    AttachItem(DISPID_Image8, m_fssImageCombo);
    AttachItem(1509, m_colorGradingCombo);
    m_hEnableEMReelCheck = ::GetDlgItem(GetHwnd(), IDC_ENABLE_EMREEL_CHECK);
    m_hEnableDecal = ::GetDlgItem(GetHwnd(), IDC_ENABLE_DECAL_CHECK);
    m_hOverwriteGlobalStereoSettingsCheck = ::GetDlgItem(GetHwnd(), IDC_GLOBAL_3DSTEREO);
    AttachItem(IDC_3D_STEREO_OFFSET_EDIT, m_3dStereoOffsetEdit);
    AttachItem(IDC_3D_STEREO_SEPARATION_EDIT, m_3dStereoSeparationEdit);
    AttachItem(IDC_3D_STEREO_ZPD_EDIT, m_3dSteroZPDEdit);
    AttachItem(IDC_COLOR_BUTTON1, m_colorButton1);
    UpdateVisuals();
    return TRUE;
}

INT_PTR BackglassVisualsProperty::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpDrawItemStruct = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
            const UINT nID = static_cast<UINT>(wParam);
            if (nID == IDC_COLOR_BUTTON1)
            {
                m_colorButton1.DrawItem(lpDrawItemStruct);
            }
            return TRUE;
        }
    }
    return DialogProcDefault(uMsg, wParam, lParam);
}
