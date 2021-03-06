#pragma once

class ComponentReader;
class Entity;
class Recipe;

#include "AssemblageHelper.h"

class AssemblageReader
{
public:
	AssemblageReader();
	virtual ~AssemblageReader();

	AssemblageHelper::E_FileStatus readAssemblageFile( Recipe** out_recipe, string p_filePath );
	AssemblageHelper::E_FileStatus parseFile( Recipe** out_recipe, ifstream* p_file );
	AssemblageHelper::E_FileStatus parseRecipe( Recipe* out_recipe, ifstream* p_file );
};

