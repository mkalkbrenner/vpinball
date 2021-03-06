#include "stdafx.h"
#include "Properties/FlipperPhysicsProperty.h"
#include <WindowsX.h>

FlipperPhysicsProperty::FlipperPhysicsProperty(VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPFLIPPER_PHYSICS, pvsel)
{
    m_physicSetList.push_back("Disable");
    m_physicSetList.push_back("Set1");
    m_physicSetList.push_back("Set2");
    m_physicSetList.push_back("Set3");
    m_physicSetList.push_back("Set4");
    m_physicSetList.push_back("Set5");
    m_physicSetList.push_back("Set6");
    m_physicSetList.push_back("Set7");
    m_physicSetList.push_back("Set8");
}

void FlipperPhysicsProperty::UpdateVisuals()
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemFlipper))
            continue;
        Flipper * const flipper = (Flipper *)m_pvsel->ElementAt(i);
        PropertyDialog::SetFloatTextbox(m_massEdit, flipper->m_d.m_mass);
        PropertyDialog::SetFloatTextbox(m_strengthEdit, flipper->m_d.m_strength);
        PropertyDialog::SetFloatTextbox(m_elasticityFalloffEdit, flipper->GetElastacityFalloff());
        PropertyDialog::SetFloatTextbox(m_returnStrengthEdit, flipper->m_d.m_return);
        PropertyDialog::SetFloatTextbox(m_coilUpRampEdit, flipper->GetRampUp());
        PropertyDialog::SetFloatTextbox(m_eosTorqueEdit, flipper->m_d.m_torqueDamping);
        PropertyDialog::SetFloatTextbox(m_eosTorqueAngleEdit, flipper->m_d.m_torqueDampingAngle);
        PropertyDialog::UpdateComboBox(m_physicSetList, m_overwriteSettingsCombo, m_physicSetList[(int)flipper->m_d.m_OverridePhysics].c_str());
        UpdateBaseVisuals(flipper, &flipper->m_d);
        flipper->UpdateUnitsInfo();
        //only show the first element on multi-select
        break;
    }
}

void FlipperPhysicsProperty::UpdateProperties(const int dispid)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemFlipper))
            continue;
        Flipper * const flipper = (Flipper *)m_pvsel->ElementAt(i);
        switch (dispid)
        {
            case 19:
                CHECK_UPDATE_ITEM(flipper->m_d.m_strength, PropertyDialog::GetFloatTextbox(m_strengthEdit), flipper);
                break;
            case 23:
                CHECK_UPDATE_VALUE_SETTER(flipper->SetReturn, flipper->GetReturn, PropertyDialog::GetFloatTextbox, m_returnStrengthEdit, flipper);
                break;
            case 27:
                CHECK_UPDATE_VALUE_SETTER(flipper->SetRampUp, flipper->GetRampUp, PropertyDialog::GetFloatTextbox, m_coilUpRampEdit, flipper);
                break;
            case 28:
                CHECK_UPDATE_VALUE_SETTER(flipper->SetElastacityFalloff, flipper->GetElastacityFalloff, PropertyDialog::GetFloatTextbox, m_elasticityFalloffEdit, flipper);
                break;
            case 113:
                CHECK_UPDATE_ITEM(flipper->m_d.m_torqueDamping, PropertyDialog::GetFloatTextbox(m_eosTorqueEdit), flipper);
                break;
            case 189:
                CHECK_UPDATE_ITEM(flipper->m_d.m_torqueDampingAngle, PropertyDialog::GetFloatTextbox(m_eosTorqueAngleEdit), flipper);
                break;
            case DISPID_Flipper_Speed:
                CHECK_UPDATE_ITEM(flipper->m_d.m_mass, PropertyDialog::GetFloatTextbox(m_massEdit), flipper);
                break;
            case 1044:
                CHECK_UPDATE_ITEM(flipper->m_d.m_OverridePhysics, (PhysicsSet)(PropertyDialog::GetComboBoxIndex(m_overwriteSettingsCombo, m_physicSetList)), flipper);
                break;
            default:
                UpdateBaseProperties(flipper, &flipper->m_d, dispid);
                break;
        }
    }
    UpdateVisuals();
}

BOOL FlipperPhysicsProperty::OnInitDialog()
{
    AttachItem(DISPID_Flipper_Speed, m_massEdit);
    AttachItem(19, m_strengthEdit);
    AttachItem(IDC_ELASTICITY_EDIT, m_elasticityEdit);
    m_baseElasticityEdit = &m_elasticityEdit;
    AttachItem(28, m_elasticityFalloffEdit);
    AttachItem(IDC_FRICTION_EDIT, m_frictionEdit);
    m_baseFrictionEdit = &m_frictionEdit;
    AttachItem(23, m_returnStrengthEdit);
    AttachItem(27, m_coilUpRampEdit);
    AttachItem(IDC_SCATTER_ANGLE_EDIT, m_scatterAngleEdit);
    m_baseScatterAngleEdit = &m_scatterAngleEdit;
    AttachItem(113, m_eosTorqueEdit);
    AttachItem(189, m_eosTorqueAngleEdit);
    AttachItem(1044, m_overwriteSettingsCombo);
    UpdateVisuals();
    return TRUE;
}

