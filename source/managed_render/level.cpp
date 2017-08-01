
#include <managed_render\\level.h>

void Level::Clear()
{
	Level::Name[0] = 0;
	Level::StrWeather = "";
	Level::StrAmbientSounds = "";
	SGeom_ModelsClear();
	SGeom_GreenClear();
	SML_LigthsClear();
	SXPhysics_UnloadGeom();
	SAIG_Clear();
	//! \todo �������� ������� ������� �������� � ������� ������
}

void Level::Load(const char* name)
{
	sprintf(Name, "%s", name);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s\\%s.lvl", GData::Pathes::Levels, name, name);
	if (!Core_0FileExists(tmppathlevel))
	{

	}

	ISXLConfig* config = Core_OpLConfig(tmppathlevel);
	if (config->KeyExists("level", "geometry"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "geometry"));
		if (Core_0FileExists(tmppath))
			SGeom_ModelsLoad(tmppath);
		else
		{
			//error
		}
	}

	if (config->KeyExists("level", "green"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "green"));
		if (Core_0FileExists(tmppath))
			SGeom_GreenLoad(tmppath);
		else
		{
			//error
		}
	}

	/*if (config->KeyExists("level", "light"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "light"));
		if (Core_0FileExists(tmppath))
			SML_LigthsLoad(tmppath);
		else
		{
			//error
		}
	}*/

	if(config->KeyExists("level", "entity"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "entity"));
		if(Core_0FileExists(tmppath))
			SXGame_LoadEnts(tmppath);
		else
		{
			//error
		}
	}

	if (config->KeyExists("level", "physic"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "physic"));
		if(Core_0FileExists(tmppath))
			SXPhysics_ImportGeom(tmppath);
		else
		{
			//error
		}
	}
	else
	{
#if defined(SX_GAME)
		SXPhysics_LoadGeom();
#endif
	}

	if (config->KeyExists("level", "aigrid"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "aigrid"));
		if (Core_0FileExists(tmppath))
			SAIG_GridLoad(tmppath);
		else
		{
			//error
		}
	}

	SGCore_LoadTexLoadTextures();

	if (config->KeyExists("level", "ambient_sounds"))
	{
		Level::StrAmbientSounds = config->GetKey("level", "ambient_sounds");

#if defined(SX_GAME)
		SGame_AmbientSndClear();
		char tmpallpath[4096];
		sprintf(tmpallpath, "%s", Level::StrAmbientSounds.c_str());
		char* tmppath = strtok(tmpallpath, " ,|");
		if (tmppath)
			SGame_AmbientSndAdd(tmppath);
		while (tmppath != NULL)
		{
			tmppath = strtok(NULL, " ,|");

			if (tmppath)
				SGame_AmbientSndAdd(tmppath);
		}

		SGame_AmbientSndPlay();
#endif
	}

	if (config->KeyExists("level", "weather"))
	{
		Level::StrWeather = config->GetKey("level", "weather");
#if defined(SX_GAME)
		char tmppath[1024];
		sprintf(tmppath, "%s%s", GData::Pathes::ConfigWeather, StrWeather.c_str());
		SGame_WeatherLoad(tmppath);

		SGame_WeatherSndPlay();
#endif
	}

	if (config->KeyExists("level", "type"))
	{
		String str = config->GetKey("level", "type");
		if (stricmp(str.c_str(), "outdoor") == 0)
		{
			ID gid = SML_LigthsCreatePoint(
				&float3(60, 60, 0),
				0,
				&float3(1, 1, 1),
				true,
				true);
			SML_LigthsSetEnable(gid, true);
			SML_LigthsSetName(gid, "sun");
		}
	}

	mem_release(config);
}

void Level::Save(const char* name)
{
	sprintf(Name, "%s", name);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s\\%s.lvl", GData::Pathes::Levels, name, name);
	if (!Core_0FileExists(tmppathlevel))
	{

	}

	FILE* file = 0;
	file = fopen(tmppathlevel, "w");

	fprintf(file, "%s", "[level]\n");

	if (SGeom_ModelsGetCount() > 0)
	{
		fprintf(file, "geometry = %s.geom\n", name);

		sprintf(tmppathlevel, "%s%s\\%s.geom", GData::Pathes::Levels, name, name);
		SGeom_ModelsSave(tmppathlevel);
	}

	if (SGeom_GreenGetCount() > 0)
	{
		fprintf(file, "green = %s.green\n", name);
		sprintf(tmppathlevel, "%s%s\\%s.green", GData::Pathes::Levels, name, name);
		SGeom_GreenSave(tmppathlevel);
	}

	if (SML_LigthsGetGlobal() > 0)
		fprintf(file, "type = outdoor\n");
	else
		fprintf(file, "type = indoor\n");

	if (SXGame_EntGetCount() > 0)
	{
		sprintf(tmppathlevel, "%s%s\\%s.ent", GData::Pathes::Levels, name, name);
		fprintf(file, "entity = %s.ent\n", name);
		SXGame_SaveEnts(tmppathlevel);
	}

	if (SAIG_GridGetCountSplits() > 0)
	{
		sprintf(tmppathlevel, "%s%s\\%s.aigrid", GData::Pathes::Levels, name, name);
		fprintf(file, "aigrid = %s.aigrid\n", name);
		SAIG_GridSave(tmppathlevel);
	}

	SXPhysics_LoadGeom();
	sprintf(tmppathlevel, "%s%s\\%s.phy", GData::Pathes::Levels, name, name);
	fprintf(file, "physic = %s.phy\n", name);
	SXPhysics_ExportGeom(tmppathlevel);

	if (Level::StrAmbientSounds[0])
	{
		fprintf(file, "ambient_sounds = %s\n", Level::StrAmbientSounds.c_str());
	}

	if (Level::StrWeather[0])
	{
		fprintf(file, "weather = %s\n", Level::StrWeather.c_str());
	}

	fprintf(file, "\n");
	fclose(file);
}

void Level::LoadParticles()
{
	char tmppathsave[1024];
	sprintf(tmppathsave, "%seff.eff", GData::Pathes::GameSource);

	SPE_EffectLoad(tmppathsave);
}

void Level::SaveParticles()
{
	char tmppathsave[1024];
	sprintf(tmppathsave, "%seff.eff", GData::Pathes::GameSource);

	SPE_EffectSave(tmppathsave);
}
