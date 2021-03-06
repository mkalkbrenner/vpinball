#include "stdafx.h"
#include "Properties/BumperPhysicsProperty.h"
#include <WindowsX.h>

BumperPhysicsProperty::BumperPhysicsProperty(VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPBUMPER_PHYSICS, pvsel)
{
}

void BumperPhysicsProperty::UpdateVisuals()
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemBumper))
            continue;
        Bumper *const bumper = (Bumper *)m_pvsel->ElementAt(i);
        PropertyDialog::SetFloatTextbox(m_forceEdit, bumper->m_d.m_force);
        m_forceEdit.EnableWindow(bumper->m_d.m_collidable);
        UpdateBaseVisuals(bumper, &bumper->m_d);
        //only show the first element on multi-select
        break;
    }
}

void BumperPhysicsProperty::UpdateProperties(const int dispid)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemBumper))
            continue;
        Bumper *const bumper = (Bumper *)m_pvsel->ElementAt(i);
        switch (dispid)
        {
            case IDC_FORCE_EDIT:
                CHECK_UPDATE_ITEM(bumper->m_d.m_force, PropertyDialog::GetFloatTextbox(m_forceEdit), bumper);
                break;
            default:
                UpdateBaseProperties(bumper, &bumper->m_d, dispid);
                break;
        }
    }
    UpdateVisuals();
}

BOOL BumperPhysicsProperty::OnInitDialog()
{
    m_hHitEventCheck = ::GetDlgItem(GetHwnd(), IDC_HAS_HITEVENT_CHECK);
    m_hCollidableCheck = ::GetDlgItem(GetHwnd(), IDC_COLLIDABLE_CHECK);
    AttachItem(IDC_HIT_THRESHOLD_EDIT, m_hitThresholdEdit);
    m_baseHitThresholdEdit = &m_hitThresholdEdit;
    AttachItem(IDC_FORCE_EDIT, m_forceEdit);
    AttachItem(IDC_SCATTER_ANGLE_EDIT, m_scatterAngleEdit);
    m_baseScatterAngleEdit = &m_scatterAngleEdit;

    UpdateVisuals();
    return TRUE;
}
