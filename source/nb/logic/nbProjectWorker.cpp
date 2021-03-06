//
// Created by Alexey Mihailov
//

#include <thread>
#include "nbProjectWorker.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include "nb/logic/factories/nbProjectEntitiesLoader.h"
#include <nb/consts/nbProjectStructConsts.h>
#include "nb/logic/request_data/nbReqProjectLoadingResult.h"

using namespace nerp;


void nbProjectWorker::loadProject(std::function<void(nbReqProjectLoadingResult)> parEndCallback,
                                  const std::string &parAbsoluteProjectPath) {
    std::string outMessage = "NONE";
    std::shared_ptr<nbOpenedProjectHandler> outOpenedProjectHandler = nullptr;
    std::string rootPath = parAbsoluteProjectPath + "/";
    try {
        if (boost::filesystem::exists(parAbsoluteProjectPath)) {
            boost::filesystem::path projectSupportFilePath = rootPath + nb_consts::FNAME_NBUILDER_SUPPORT;
            if (boost::filesystem::exists(projectSupportFilePath)) {
                YAML::Node projectSupportFile = YAML::LoadFile(projectSupportFilePath.string());
                std::shared_ptr<nbProject> openedProject = nbProjectEntitiesLoader::getProjectFromYAML(
                        projectSupportFile);
                outOpenedProjectHandler = std::make_shared<nbOpenedProjectHandler>(rootPath,
                                                                                   projectSupportFilePath.string(),
                                                                                   rootPath +
                                                                                   nb_consts::FNAME_GEN_MACROS_DEFINITIONS,
                                                                                   rootPath +
                                                                                   nb_consts::DIRNAME_TEMPLATE_RECIPES + "/",
                                                                                   rootPath + nb_consts::DIRNAME_USER_RECIPES +
                                                                                   "/", openedProject);
                discoverProjectRecipes(*outOpenedProjectHandler);
            } else {
                outMessage = "Project file descriptor not exists";
            }
        } else {
            outMessage = "Path not exists";
        }
    } catch (std::runtime_error &err) {
        outMessage = err.what();
    }


    auto loadingResult = nbReqProjectLoadingResult(outOpenedProjectHandler, outMessage);
    parEndCallback(std::move(loadingResult));
}

void nbProjectWorker::discoverProjectRecipes(nbOpenedProjectHandler &parPrjHandler) {
    const auto is_extension_fn = [](const boost::filesystem::directory_entry& entry)
            {return (entry.path().has_extension() && entry.path().extension() == nb_consts::FMASK_EXTENSION_RECIPE);};
    const auto process_recipe_file_fn = [](nbOpenedProjectHandler &parPrjHandler, const boost::filesystem::directory_entry& entry,
                                  nbERecipeKind recipeKind){
        try {
            auto readRecipe = nbProjectEntitiesLoader::getProjectRecipe(parPrjHandler, recipeKind, YAML::LoadFile(entry.path().string()));
            addRecipeToProject(parPrjHandler, readRecipe);
        } catch (...){}
    };
    const auto find_and_process_recipes_fn = [is_extension_fn = is_extension_fn,
                                              process_recipe_file_fn = process_recipe_file_fn]
            (nbOpenedProjectHandler &parPrjHandler, const boost::filesystem::directory_iterator& parDirIter, nbERecipeKind parRecipeKind){
        for (const auto& recipeFile : parDirIter){
            if (is_extension_fn(recipeFile)){
                process_recipe_file_fn(parPrjHandler, recipeFile, parRecipeKind);
            }
        }
    };


    find_and_process_recipes_fn(parPrjHandler, getTemplateRecipesIterator(parPrjHandler), nbERecipeKind::TemplateRecipe);
    find_and_process_recipes_fn(parPrjHandler, getUserRecipesIterator(parPrjHandler), nbERecipeKind::UserRecipe);

}

boost::filesystem::directory_iterator getDirIterator(const std::string& parPath){
    if (!(boost::filesystem::exists(parPath) && boost::filesystem::is_directory(parPath))){
        boost::filesystem::create_directory(parPath);
    }

    return boost::filesystem::directory_iterator(parPath);
}

boost::filesystem::directory_iterator nbProjectWorker::getTemplateRecipesIterator(nbOpenedProjectHandler &parPrjHandler) {
    const std::string& targetPath = parPrjHandler.PathRootTemplateRecipes;
    return getDirIterator(targetPath);
}

boost::filesystem::directory_iterator nbProjectWorker::getUserRecipesIterator(nbOpenedProjectHandler &parPrjHandler) {
    const std::string& targetPath = parPrjHandler.PathRootUserRecipes;
    return getDirIterator(targetPath);
}

void nbProjectWorker::addRecipeToProject(nbOpenedProjectHandler &parPrjHandler, std::shared_ptr<nbRecipe> parRecipe) {
    parPrjHandler.addRecipeToProject(std::move(parRecipe));
}



