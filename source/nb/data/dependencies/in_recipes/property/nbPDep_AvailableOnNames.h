//
// Copyright (c) Alexey "LexPest" Mihailov. All rights reserved.
//
// Use of this source code is governed by an MIT license
// that can be found in the LICENSE file.

#ifndef NERP_BUILDER_WX_NBPDEP_AVAILABLEONNAMES_H
#define NERP_BUILDER_WX_NBPDEP_AVAILABLEONNAMES_H

#include <memory>
#include <nb/data/props/nbProperty.h>
#include <list>
#include <utility>
#include <yaml-cpp/node/node.h>
#include <nb/data/dependencies/in_recipes/nbTargetsContainedDependency.h>
#include <nb/data/dependencies/in_recipes/nbDependency_InRecipe.h>
#include <nb/data/features/nbFeature.h>
#include <nb/data/dependencies/in_recipes/nbEDepGroupKind_InRecipe_DepTarget.h>
#include <nb/data/dependencies/in_recipes/nbEDepGroupKind_InRecipe_DepParam.h>
#include <nb/data/dependencies/in_recipes/nbEDepGroupKind_InRecipe_ByDepParam.h>

//namespace nerp {
//    class nbPDep_AvailableOnNames : public nbDependency_InRecipe<nbEDepGroupKind_InRecipe_DepTarget::Property,
//            nbEDepGroupKind_InRecipe_DepParam::Availability, nbEDepGroupKind_InRecipe_ByDepParam::Availability>
//            , public nbTargetsContainedDependency<nbProperty> {
//    public:
//         const std::function<bool()> &getIsSatisfiedFunction() override;
//
//    };
//}


#endif //NERP_BUILDER_WX_NBPDEP_AVAILABLEONNAMES_H
