#include "param_patch.h"

#include <cstdint>

#include <coresystem/cs_param.hpp>
#include <param/param.hpp>

#include "logging.h"

namespace
{
constexpr int PARAM_WAIT_TIMEOUT_MS = 120000;

struct PatchStats
{
    uint32_t totalRows = 0;
    uint32_t changedRows = 0;
    uint32_t alreadyZeroRows = 0;
};

bool HasAnyRequirement(const from::paramdef::EQUIP_PARAM_WEAPON_ST& weapon)
{
    return weapon.properStrength != 0 || weapon.properAgility != 0 ||
           weapon.properMagic != 0 || weapon.properFaith != 0 ||
           weapon.properLuck != 0;
}

PatchStats PatchEquipParamWeapon()
{
    PatchStats stats{};

    for (auto [rowId, weapon] : from::param::EquipParamWeapon)
    {
        (void)rowId;
        ++stats.totalRows;

        if (!HasAnyRequirement(weapon))
        {
            ++stats.alreadyZeroRows;
            continue;
        }

        weapon.properStrength = 0;
        weapon.properAgility = 0;
        weapon.properMagic = 0;
        weapon.properFaith = 0;
        weapon.properLuck = 0;
        ++stats.changedRows;
    }

    return stats;
}
}

void InitRuntimeParamPatch()
{
    logging::Debug("Waiting for libER param repository");

    if (!from::CS::SoloParamRepository::wait_for_params(PARAM_WAIT_TIMEOUT_MS))
    {
        logging::Debug("Timed out waiting for libER param repository");
        return;
    }

    auto repository = from::CS::SoloParamRepository::instance();
    if (!repository)
    {
        logging::Debug("libER param repository instance unavailable after wait");
        return;
    }

    logging::Debug("libER param repository ready at %p",
                   static_cast<void*>(&repository.reference()));

    const PatchStats stats = PatchEquipParamWeapon();
    if (stats.totalRows == 0)
    {
        logging::Debug("EquipParamWeapon table was empty or unavailable");
        return;
    }

    logging::Debug(
        "EquipParamWeapon requirements patched: totalRows=%u changedRows=%u alreadyZeroRows=%u",
        stats.totalRows,
        stats.changedRows,
        stats.alreadyZeroRows);
}
