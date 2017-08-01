
LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void SXLevelEditor::LevelNew()
{
	if (SGeom_ModelsGetCount() <= 0 || (SGeom_ModelsGetCount() > 0 && MessageBox(0, "Are you sure you need to create a new level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO))
		return;

	Level::Clear();
	char tmpcaption[256];
	sprintf(tmpcaption, "%s: new level ** ", EDITORS_LEVEL_CAPTION);
	SXLevelEditor::JobWindow->SetText(tmpcaption);
	GData::Editors::ActiveGroupType = 0;
	GData::Editors::ActiveElement = -1;
	GData::Editors::ActiveGreenSplit = -1;
	GData::Editors::ActiveGreenObject = -1;
}

void SXLevelEditor::LevelOpen()
{
	if (SGeom_ModelsGetCount() > 0 && MessageBox(0, "Are you sure that you need to open the level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)
		return;

	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Levels, FILE_FILTER_LEVEL);
	if (def_str_validate(tmppath))
	{
		StrCutNameNEx(tmppath, tmpname);
		Level::Load(tmpname);
		char tmpcaption[256];
		sprintf(tmpcaption, "%s: %s", EDITORS_LEVEL_CAPTION, tmpname);
		SXLevelEditor::JobWindow->SetText(tmpcaption);
	}
}

void SXLevelEditor::LevelSave()
{
	if (Level::Name[0])
		Level::Save(Level::Name);
	else
	{
		if (SGeom_ModelsGetCount() <= 0)
			MessageBox(0, "You need to create a level!", 0, 0);
		else
			SXLevelEditor::LevelSaveAs();
	}
}

void SXLevelEditor::LevelSaveAs()
{
	if (SGeom_ModelsGetCount() <= 0)
	{
		MessageBox(0, "You need to create a level!", 0, 0);
		return;
	}

	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_SAVE, tmppath, 0, GData::Pathes::Levels, FILE_FILTER_LEVEL);
	if (StrValidate(tmppath))
	{
		StrCutNameNEx(tmppath, tmpname);
		Level::Save(tmpname);
		char tmpcaption[256];
		sprintf(tmpcaption, "%s: %s", EDITORS_LEVEL_CAPTION, tmpname);
		SXLevelEditor::JobWindow->SetText(tmpcaption);
	}
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//����
	//�����
	if (id == ID_FILE_NEW)
	{
		SXLevelEditor::LevelNew();
	}
	//�������
	else if (id == ID_FILE_OPEN)
	{
		SXLevelEditor::LevelOpen();
	}
	//���������
	else if (id == ID_FILE_SAVE)
	{
		SXLevelEditor::LevelSave();
	}
	//��������� ���
	else if (id == ID_FILE_SAVEAS)
	{
		SXLevelEditor::LevelSaveAs();
	}
	//�����
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hwnd, msg, wParam, lParam);
	}

	else if (id == ID_VIEW_GRID)
	{
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
		GData::Editors::RenderGrid = SXLevelEditor::MainMenu->GetCheckedItem(id);
	}
	else if (id == ID_VIEW_AXES)
	{
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
		GData::Editors::RenderAxesStatic = SXLevelEditor::MainMenu->GetCheckedItem(id);
	}

	//
	else if (id == ID_FINALIMAGE_COLOR)
	{
		GData::FinalImage = DS_RT::ds_rt_color;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		GData::FinalImage = DS_RT::ds_rt_normal;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		GData::FinalImage = DS_RT::ds_rt_param;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		GData::FinalImage = DS_RT::ds_rt_ambient_diff;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		GData::FinalImage = DS_RT::ds_rt_specular;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		GData::FinalImage = DS_RT::ds_rt_scene_light_com;
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}


	//
	else if (id == ID_SELECTIONSETTINGS_SELECTION)
	{
		GData::Editors::SelSelection = !SXLevelEditor::MainMenu->GetCheckedItem(id);
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_ZTEST)
	{
		GData::Editors::SelZTest = !SXLevelEditor::MainMenu->GetCheckedItem(id);
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_MESH)
	{
		GData::Editors::SelMesh = !SXLevelEditor::MainMenu->GetCheckedItem(id);
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_BACKFACESCULL)
	{
		GData::Editors::SelBackFacesCull = !SXLevelEditor::MainMenu->GetCheckedItem(id);
		SXLevelEditor::MainMenu->CheckItem(id, !SXLevelEditor::MainMenu->GetCheckedItem(id));
	}

	else if (id >= SX_LE_MMENU_WEATHER_BEGIN_ID)
	{
		for (int i = 0; i < SXLevelEditor::MenuWeatherCount; ++i)
		{
			SXLevelEditor::MainMenu->CheckItem(SX_LE_MMENU_WEATHER_BEGIN_ID+i, false);
		}
		SXLevelEditor::MainMenu->CheckItem(id, true);
		SXLevelEditor::MenuWeatherCurrID = (id - SX_LE_MMENU_WEATHER_BEGIN_ID) - 1;

		if (SXLevelEditor::MenuWeatherCurrID >= 0)
		{
			char tmppath[1024];
			sprintf(tmppath, "%s%s", GData::Pathes::ConfigWeather, SXLevelEditor::MenuWeatherArr[SXLevelEditor::MenuWeatherCurrID]);
			SGame_WeatherLoad(tmppath);
		}
		else
		{
			SGame_WeatherLoad(0);

			ID gid = SML_LigthsGetGlobal();
			if (gid >= 0)
			{
				SML_LigthsSetPos(gid, &float3(60, 60, 0), false);
				SML_LigthsSetColor(gid, &float3(1, 1, 1));
				SML_LigthsSetEnable(gid, true);
			}
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GData::ReSize = true;
	return 0;
}

///////

LRESULT SXLevelEditor_RenderWindow_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	GData::Editors::ObjAxesHelper->OnMouseMove(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));

	if (GData::Editors::ObjAxesHelper->m_bIsDragging == false)
		return 0;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && GData::Editors::ActiveElement >= 0)
	{
		if (GData::Editors::ObjAxesHelper->GetType() == AxesHelper::HT_MOVE)
		{
			float3* pos = SGeom_ModelsMGetPosition(GData::Editors::ActiveElement);
			float3 npos = (*pos) + (GData::Editors::ObjAxesHelper->GetPosition() - SXLevelEditor::HelperPos);
			if ((*pos).x != npos.x || (*pos).y != npos.y || (*pos).z != npos.z)
			{
				*pos = npos;
				
				SGeom_ModelsMApplyTransform(GData::Editors::ActiveElement);
				float3 min, max;
				SGeom_ModelsMGetMinMax(GData::Editors::ActiveElement, &min, &max);

				SXLevelEditor::HelperPos = (max + min) * 0.5f;
			}
		}
		else if (GData::Editors::ObjAxesHelper->GetType() == AxesHelper::HT_ROTATE)
		{
			float3* rot = SGeom_ModelsMGetRotation(GData::Editors::ActiveElement);
			float3 nrot = GData::Editors::ObjAxesHelper->GetRotation();
			if ((*rot).x != nrot.x || (*rot).y != nrot.y || (*rot).z != nrot.z)
			{
				*rot = GData::Editors::ObjAxesHelper->GetRotation();
				SGeom_ModelsMApplyTransform(GData::Editors::ActiveElement);
			}
		}
		else if (GData::Editors::ObjAxesHelper->GetType() == AxesHelper::HT_SCALE)
		{
			float3* scale = SGeom_ModelsMGetScale(GData::Editors::ActiveElement);
			float3 nscale = GData::Editors::ObjAxesHelper->GetScale() - float3(1,1,1);
			if ((*scale).x != nscale.x || (*scale).y != nscale.y || (*scale).z != nscale.z)
			{
				*scale += nscale;
				SGeom_ModelsMApplyTransform(GData::Editors::ActiveElement);
			}
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && GData::Editors::ActiveElement >= 0 && GData::Editors::ActiveGreenSplit >= 0 && GData::Editors::ActiveGreenObject >= 0)
	{
		float3_t pos;
		SGeom_GreenGetPosObject(GData::Editors::ActiveElement, GData::Editors::ActiveGreenSplit, GData::Editors::ActiveGreenObject, &pos);
		float3 helperpos = GData::Editors::ObjAxesHelper->GetPosition();
		if (pos.x != helperpos.x || pos.y != helperpos.y || pos.z != helperpos.z)
		{
			SGeom_GreenSetPosObject(GData::Editors::ActiveElement, &GData::Editors::ActiveGreenSplit, &GData::Editors::ActiveGreenObject, &(float3_t)helperpos);
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && GData::Editors::ActiveElement >= 0)
	{
		SXbaseEntity* bEnt = SXGame_EntGet(GData::Editors::ActiveElement);
		if (!bEnt)
			return 0;

		if (GData::Editors::ObjAxesHelper->GetType() == AxesHelper::HT_MOVE)
			bEnt->SetPos(GData::Editors::ObjAxesHelper->GetPosition());
		else if (GData::Editors::ObjAxesHelper->GetType() == AxesHelper::HT_ROTATE)
			bEnt->SetOrient(GData::Editors::ObjAxesHelper->GetRotationQ());

		SXLevelEditor::GameUpdatePosRot();
	}
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;
	GData::Editors::ObjAxesHelper->m_bIsDragging = true;
	GData::Editors::ObjAxesHelper->m_bIsDraggingStart = true;
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	GData::Editors::ObjAxesHelper->m_bIsDragging = false;
	GData::Editors::ObjAxesHelper->m_bIsDraggingStop = true;

	/*if (!SSInput_GetKeyState(SIK_LALT))
		return 0;*/

	float3 _res;
	float3_t pos2;
	ID idmodel;
	ID idmtl;

	float3 camDir;
	float det;
	SMMATRIX mat = SMMatrixInverse(&det, GData::MCamView);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(GData::Handle3D, &pt);

	float3 pos = float3(
		(2.0f * (float)pt.x / GData::WinSize.x - 1.0f) / GData::MCamProj._11,
		-(2.0f * (float)pt.y / GData::WinSize.y - 1.0f) / GData::MCamProj._22,
		1.0f
		) * mat;
	camDir = pos - GData::ConstCurrCamPos;

	if (!SXLevelEditor::CheckBoxTBArrow->GetCheck())
		return 0;

	SXLevelEditor::IdMtl = -1;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || GData::Editors::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
		{
			SXLevelEditor::ListBoxList->SetSel(idmodel);
			SXLevelEditor::GeomSel(idmodel);
			SXLevelEditor::IdMtl = idmtl;
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && GData::Editors::ActiveElement >= 0)
	{
		ID idgreen;
		ID idsplit;
		ID idobj;

		if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 0)
		{
			if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_GreenTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				GData::Editors::ActiveGreenSplit = idsplit;
				GData::Editors::ActiveGreenObject = idobj;
				SGeom_GreenGetPosObject(idgreen, idsplit, idobj, &pos2);

				SXLevelEditor::StaticGreenSelX->SetText("Pos X:");
				SXLevelEditor::StaticGreenSelY->SetText("Pos Y:");
				SXLevelEditor::StaticGreenSelZ->SetText("Pos Z:");

				SXLevelEditor::EditGreenSelX->SetText(String(pos2.x).c_str());
				SXLevelEditor::EditGreenSelY->SetText(String(pos2.y).c_str());
				SXLevelEditor::EditGreenSelZ->SetText(String(pos2.z).c_str());
				
				GData::Editors::ObjAxesHelper->SetPosition(pos2);
				GData::Editors::ObjAxesHelper->SetRotation(float3(0,0,0));
				GData::Editors::ObjAxesHelper->SetScale(float3(1, 1, 1));

				SXLevelEditor::IdMtl = idmtl;
			}
		}
		else if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 1)
		{
			if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				idobj = SGeom_GreenAddObject(0, &_res, &idsplit);
				GData::Editors::ActiveGreenSplit = idsplit;
				GData::Editors::ActiveGreenObject = idobj;

				SXLevelEditor::StaticGreenSelX->SetText("Pos X:");
				SXLevelEditor::StaticGreenSelY->SetText("Pos Y:");
				SXLevelEditor::StaticGreenSelZ->SetText("Pos Z:");

				SXLevelEditor::EditGreenSelX->SetText(String(_res.x).c_str());
				SXLevelEditor::EditGreenSelY->SetText(String(_res.y).c_str());
				SXLevelEditor::EditGreenSelZ->SetText(String(_res.z).c_str());

				GData::Editors::ObjAxesHelper->SetPosition(pos2);
				GData::Editors::ObjAxesHelper->SetRotation(float3(0, 0, 0));
				GData::Editors::ObjAxesHelper->SetScale(float3(1, 1, 1));
				
				int qwert = 0;
			}
		}

		else if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 2)
		{
			if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				GData::Editors::GreenBoxPos = _res;
				int density = SXLevelEditor::TrackBarGreenDensity->GetPos();

				float3 min, max, npos;
				min.x = _res.x - GData::Editors::GreenBoxWHD.x * 0.5f;
				min.z = _res.z - GData::Editors::GreenBoxWHD.z * 0.5f;

				max.x = _res.x + GData::Editors::GreenBoxWHD.x * 0.5f;
				max.z = _res.z + GData::Editors::GreenBoxWHD.z * 0.5f;

				npos.y = _res.y + GData::Editors::GreenBoxWHD.y;

				for (int i = 0; i < density; ++i)
				{
					npos.x = randf(min.x, max.x);
					npos.z = randf(min.z, max.z);
					if(SGeom_ModelsTraceBeam(&npos, &float3(0, -1, 0), &_res, &idmodel, &idmtl))
						SGeom_GreenAddObject(GData::Editors::ActiveElement, &_res, 0);
				}
			}
		}

		else if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 3)
		{
			if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_GreenTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				GData::Editors::ActiveGreenSplit = -1;
				GData::Editors::ActiveGreenObject = -1;
				SXLevelEditor::EditGreenSelX->SetText("");
				SXLevelEditor::EditGreenSelY->SetText("");
				SXLevelEditor::EditGreenSelZ->SetText("");
				SGeom_GreenDelObject(idgreen, idsplit, idobj);
				int qwert = 0;
			}
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && GData::Editors::ActiveElement >= 0)
	{

	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SXLevelEditor::RadioButtonAIQuadAdd->GetCheck())
		{
			float3 start = pos;
			float3 dir = camDir;
			float3 end = start + dir * 1000.0f;
			btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
			SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

			if (cb.hasHit())
			{
				SAIG_QuadAdd(&BTVEC_F3(cb.m_hitPointWorld));
			}
		}
		else if (SXLevelEditor::RadioButtonAIQuadsMSel->GetCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadSelect(idaq, true);
		}
		else if (SXLevelEditor::RadioButtonAIQuadsSelDel->GetCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadDelete(idaq);
		}
		else if (SXLevelEditor::RadioButtonAIGPAdd->GetCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_GraphPointAdd(idaq);
		}
		else if (SXLevelEditor::RadioButtonAIGPDel->GetCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_GraphPointDelete(idaq);
		}
	}
	
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_RClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	float3 camDir;
	float det;
	SMMATRIX mat = SMMatrixInverse(&det, GData::MCamView);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(GData::Handle3D, &pt);

	float3 pos = float3(
		(2.0f * (float)pt.x / GData::WinSize.x - 1.0f) / GData::MCamProj._11,
		-(2.0f * (float)pt.y / GData::WinSize.y - 1.0f) / GData::MCamProj._22,
		1.0f
		) * mat;
	camDir = pos - GData::ConstCurrCamPos;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && GData::Editors::ActiveElement >= 0)
	{
		float3 _res;
		float3_t pos2;
		ID idmodel;
		ID idmtl;

		ID idgreen;
		ID idsplit;
		ID idobj;

		if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 2)
		{
			if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
				GData::Editors::GreenBoxPos = _res;
		}
	}

	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SXLevelEditor::RadioButtonAIQuadsMSel->GetCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadSelect(idaq, false);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_MBUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		SAIG_QuadSelect(-1, false);
	}

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_Delete(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && GData::Editors::ActiveElement >= 0 && GData::Editors::ActiveGreenSplit >= 0 && GData::Editors::ActiveGreenObject >= 0)
	{
		SGeom_GreenDelObject(GData::Editors::ActiveElement, GData::Editors::ActiveGreenSplit, GData::Editors::ActiveGreenObject);
		GData::Editors::ActiveGreenSplit = -1;
		GData::Editors::ActiveGreenObject = -1;
		SXLevelEditor::EditGreenSelX->SetText("");
		SXLevelEditor::EditGreenSelY->SetText("");
		SXLevelEditor::EditGreenSelZ->SetText("");
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();
	long tmpcountmodel = SGeom_ModelsGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel;++i)
	{
		sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelsMGetName(i), SGeom_ModelsMGetCountPoly(i));
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);
	}

	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GameActivateAll(false);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GeomActivateCreate(true);
	//SXLevelEditor::GeomActivateTrans(false);

	GData::Editors::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GEOM;
	GData::Editors::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();
	long tmpcountmodel = SGeom_GreenGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel; ++i)
	{
		sprintf(tmpnamecountpoly, "%s | %s | %d", 
			SGeom_GreenMGetName(i),
			(SGeom_GreenMGetTypeCountGen(i) == GeomGreenType::ggt_grass ? "grass" : "tree/shrub"),
			SGeom_GreenMGetCountGen(i));
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);
	}

	SXLevelEditor::GeomActivateAll(false);

	SXLevelEditor::GameActivateAll(false);

	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GreenActivateCreate(true);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GreenActivateMain(true);
	
	//MCActivateTrans(false);

	GData::Editors::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GREEN;
	GData::Editors::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();

	int tmpcoungo = SXGame_EntGetCount();
	int tmpcoungo2 = 0;
	char tmpname[1024];
	for (int i = 0; i < tmpcoungo; ++i)
	{
		SXbaseEntity* bEnt = SXGame_EntGet(i);
		if (bEnt)
		{
			sprintf(tmpname, "%s / %s", bEnt->GetName(), bEnt->GetClassName());
			SXLevelEditor::ListBoxList->AddItem(tmpname);
			SXLevelEditor::ListBoxList->SetItemData(SXLevelEditor::ListBoxList->GetCountItem() - 1, i);
			++tmpcoungo2;
		}
	}

	char tmptextvalcountgo[64];
	sprintf(tmptextvalcountgo, "%d", tmpcoungo2);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountgo);

	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GameActivateAll(true);

	GData::Editors::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GAME;
	GData::Editors::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();

	SXLevelEditor::StaticListValCount->SetText("0");

	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GameActivateAll(false);

	SXLevelEditor::AIGridActivateAll(true);

	GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_AIGRID;
	GData::Editors::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ListBoxList_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();

	GData::Editors::ActiveGreenSplit = -1;
	GData::Editors::ActiveGreenObject = -1;

	if (sel < 0)
		return 0;

	if (GData::Editors::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM || GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
			SXLevelEditor::GeomSel(sel);
	}
	else if (GData::Editors::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GREEN || GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
			SXLevelEditor::GreenSel(sel);
	}
	else if (GData::Editors::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GAME || GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
			SXLevelEditor::GameSel(sel);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();

	if (sel < 0)
		return 0;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
		{
			SGeom_ModelsDelModel(sel);
			SXLevelEditor::ListBoxList->DeleteItem(sel);
			if (SGeom_ModelsGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_ModelsGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->SetSel(sel);
				SXLevelEditor::GeomSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGeometryOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
		{
			SGeom_GreenDelGreen(sel);
			SXLevelEditor::ListBoxList->DeleteItem(sel);
			if (SGeom_GreenGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_GreenGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->SetSel(sel);
				SXLevelEditor::GreenSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGreenOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
		{
			SXGame_RemoveEntity(SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel)));
			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);

			if (sel >= SXLevelEditor::ListBoxList->GetCountItem())
				--sel;
			if (sel >= 0)
			{
				SXLevelEditor::ListBoxList->SetSel(sel);
				SXLevelEditor::GameSel(sel);
			}
		}
	}

	return 0;
}





LRESULT SXLevelEditor_GroupBox_Notify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (((NMHDR*)lParam)->hwndFrom == SXLevelEditor::ListViewGameClass->GetHWND() && ((NMHDR*)lParam)->code == NM_CLICK)
	{
		SXLevelEditor_ListViewGameClass_Click();
	}
	return 0;
}

LRESULT SXLevelEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXLevelEditor::CheckBoxTBArrow->GetHWND() == handle_elem)
		{
			SXLevelEditor::CheckBoxTBPos->SetCheck(false);
			SXLevelEditor::CheckBoxTBRot->SetCheck(false);
			SXLevelEditor::CheckBoxTBScale->SetCheck(false);
			GData::Editors::ObjAxesHelper->SetType(AxesHelper::HT_NONE);
		}
		else if (SXLevelEditor::CheckBoxTBPos->GetHWND() == handle_elem)
		{
			if (
				GData::Editors::ActiveElement >= 0 &&
				(
				GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM ||
				(GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && GData::Editors::ActiveGreenSplit >= 0 && GData::Editors::ActiveGreenObject >= 0) ||
				GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME
				)
				)
			{
				SXLevelEditor::CheckBoxTBArrow->SetCheck(false);
				SXLevelEditor::CheckBoxTBRot->SetCheck(false);
				SXLevelEditor::CheckBoxTBScale->SetCheck(false);
				GData::Editors::ObjAxesHelper->SetType(AxesHelper::HT_MOVE);
			}
			else
				SXLevelEditor::CheckBoxTBPos->SetCheck(false);
		}
		else if (SXLevelEditor::CheckBoxTBRot->GetHWND() == handle_elem)
		{
			if ((GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && GData::Editors::ActiveElement >= 0)
			{
				SXLevelEditor::CheckBoxTBArrow->SetCheck(false);
				SXLevelEditor::CheckBoxTBPos->SetCheck(false);
				SXLevelEditor::CheckBoxTBScale->SetCheck(false);
				GData::Editors::ObjAxesHelper->SetType(AxesHelper::HT_ROTATE);
			}
			else
				SXLevelEditor::CheckBoxTBRot->SetCheck(false);
		}
		else if (SXLevelEditor::CheckBoxTBScale->GetHWND() == handle_elem)
		{
			if ((GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && GData::Editors::ActiveElement >= 0)
			{
				SXLevelEditor::CheckBoxTBPos->SetCheck(false);
				SXLevelEditor::CheckBoxTBRot->SetCheck(false);
				SXLevelEditor::CheckBoxTBArrow->SetCheck(false);
				GData::Editors::ObjAxesHelper->SetType(AxesHelper::HT_SCALE);
			}
			else
				SXLevelEditor::CheckBoxTBScale->SetCheck(false);
		}

		else if (SXLevelEditor::ButtonTBNew->GetHWND() == handle_elem)
		{
			SXLevelEditor::LevelNew();
		}
		else if (SXLevelEditor::ButtonTBOpen->GetHWND() == handle_elem)
		{
			SXLevelEditor::LevelOpen();
		}
		else if (SXLevelEditor::ButtonTBSave->GetHWND() == handle_elem)
		{
			SXLevelEditor::LevelSave();
		}
		else if (SXLevelEditor::ButtonTBSaveAs->GetHWND() == handle_elem)
		{
			SXLevelEditor::LevelSaveAs();
		}

		else if (SXLevelEditor::CheckBoxTBGrid->GetHWND() == handle_elem)
		{
			SXLevelEditor::MainMenu->CheckItem(ID_VIEW_GRID, SXLevelEditor::CheckBoxTBGrid->GetCheck());
			GData::Editors::RenderGrid = SXLevelEditor::CheckBoxTBGrid->GetCheck();
		}
		else if (SXLevelEditor::CheckBoxTBAxes->GetHWND() == handle_elem)
		{
			SXLevelEditor::MainMenu->CheckItem(ID_VIEW_AXES, SXLevelEditor::CheckBoxTBAxes->GetCheck());
			GData::Editors::RenderAxesStatic = SXLevelEditor::CheckBoxTBAxes->GetCheck();
		}

		else if (SXLevelEditor::CheckBoxTBRColor->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, true);
			SXLevelEditor::CheckBoxTBRColor->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_color;
		}
		else if (SXLevelEditor::CheckBoxTBRNormal->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_NORMALS, true);
			SXLevelEditor::CheckBoxTBRNormal->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_normal;
		}
		else if (SXLevelEditor::CheckBoxTBRParam->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXLevelEditor::CheckBoxTBRParam->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_param;
		}
		else if (SXLevelEditor::CheckBoxTBRAmDiff->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXLevelEditor::CheckBoxTBRAmDiff->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_ambient_diff;
		}
		else if (SXLevelEditor::CheckBoxTBRSpecular->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXLevelEditor::CheckBoxTBRSpecular->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_specular;
		}
		else if (SXLevelEditor::CheckBoxTBRLighting->GetHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXLevelEditor::CheckBoxTBRLighting->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_scene_light_com;
		}

		else if (SXLevelEditor::CheckBoxTBSelS->GetHWND() == handle_elem)
		{
			GData::Editors::SelSelection = SXLevelEditor::CheckBoxTBSelS->GetCheck();
			SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, SXLevelEditor::CheckBoxTBSelS->GetCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelZTest->GetHWND() == handle_elem)
		{
			GData::Editors::SelZTest = SXLevelEditor::CheckBoxTBSelZTest->GetCheck();
			SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_ZTEST, SXLevelEditor::CheckBoxTBSelZTest->GetCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelMesh->GetHWND() == handle_elem)
		{
			GData::Editors::SelMesh = SXLevelEditor::CheckBoxTBSelMesh->GetCheck();
			SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, SXLevelEditor::CheckBoxTBSelMesh->GetCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelCullBack->GetHWND() == handle_elem)
		{
			GData::Editors::SelBackFacesCull = SXLevelEditor::CheckBoxTBSelCullBack->GetCheck();
			SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, SXLevelEditor::CheckBoxTBSelCullBack->GetCheck());
		}

		else if (SXLevelEditor::CheckBoxTBAIGBound->GetHWND() == handle_elem)
		{
			GData::Editors::AIGBound = SXLevelEditor::CheckBoxTBAIGBound->GetCheck();
		}
		else if (SXLevelEditor::CheckBoxTBAIGQuad->GetHWND() == handle_elem)
		{
			GData::Editors::AIGQuad = SXLevelEditor::CheckBoxTBAIGQuad->GetCheck();
		}
		else if (SXLevelEditor::CheckBoxTBAIGGraphPoint->GetHWND() == handle_elem)
		{
			GData::Editors::AIGGraphPoint = SXLevelEditor::CheckBoxTBAIGGraphPoint->GetCheck();
		}

		else if (SXLevelEditor::CheckBoxTBLevelType->GetHWND() == handle_elem)
		{
			if (SXLevelEditor::CheckBoxTBLevelType->GetCheck())
			{
				ID gid = SML_LigthsGetGlobal();
				if (gid < 0)
				{
					gid = SML_LigthsCreatePoint(
						&float3(60, 60, 0),
						0,
						&float3(1, 1, 1),
						true,
						true);
					SML_LigthsSetEnable(gid, true);
					SML_LigthsSetName(gid, "sun");
				}
			}
			else
			{
				ID gid = SML_LigthsGetGlobal();
				if (gid >= 0)
					SML_LigthsDeleteLight(gid);
			}
		}

		else if (SXLevelEditor::CheckBoxTBGLightEnable->GetHWND() == handle_elem)
		{
			ID gid = SML_LigthsGetGlobal();
			if (gid >= 0)
				SML_LigthsSetEnable(gid, SXLevelEditor::CheckBoxTBGLightEnable->GetCheck());
			else
				SXLevelEditor::CheckBoxTBGLightEnable->SetCheck(false);
		}
	}

	return 0;
}


LRESULT SXLevelEditor_GroupBox_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (handle_elem == SXLevelEditor::CheckBoxAIGridMarkedSplits->GetHWND())
			SAIG_GridSetMarkSplits(SXLevelEditor::CheckBoxAIGridMarkedSplits->GetCheck());
	}
	else if (Notification == CBN_SELCHANGE)
	{
		if (SXLevelEditor::ComboBoxGameValue->GetHWND() == handle_elem)
		{
			int sel = SXLevelEditor::ListBoxList->GetSel();
			char txt[256];
			SXLevelEditor::ComboBoxGameValue->GetItemText(SXLevelEditor::ComboBoxGameValue->GetSel(), txt);
			SXLevelEditor::ListViewGameClass->SetTextItem(txt, 1, SXLevelEditor::ListViewGameClass->GetSelString());
			SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
			if (bEnt)
			{
				propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(SXLevelEditor::ListViewGameClass->GetSelString());
				SXLevelEditor::ComboBoxGameValue->GetItemData(SXLevelEditor::ComboBoxGameValue->GetSel());
				bEnt->SetKV(pd->szKey, (const char*)SXLevelEditor::ComboBoxGameValue->GetItemData(SXLevelEditor::ComboBoxGameValue->GetSel()));
			}
		}
		else if (SXLevelEditor::ComboBoxGameClass->GetHWND() == handle_elem)
		{
			if (SXLevelEditor::ComboBoxGameClass->GetSel() == 0)
				return 0;

			int sel = SXLevelEditor::ListBoxList->GetSel();

			if (sel < 0)
				return 0;

			SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
			proptable_t* pt = SXGame_EntGetProptable(bEnt->GetClassName());

			propdata_t* pd;
			char txtkey[256];
			char txtval[256];

			struct KeyVal
			{
				KeyVal(){}
				KeyVal(const char* _key, const char* _val)
				{
					key = _key;
					val = _val;
				}

				String key;
				String val;
			};

			Array<KeyVal> tmparrdata;

			Array<proptable_t*> tmparr;

			proptable_t* ptparent = pt->pBaseProptable;
			while (ptparent)
			{
				tmparr.push_back(ptparent);
				ptparent = ptparent->pBaseProptable;
			}

			for (int k = 0; k < tmparr.size(); ++k)
			{
				ptparent = tmparr[(tmparr.size() - 1) - k];
				for (int i = 0; i < ptparent->numFields; ++i)
				{
					pd = &ptparent->pData[i];
					if (pd->szKey && pd->szEdName && !(pd->flags & PDFF_NOEDIT) && pd->editor.type != PDE_NONE)
					{
						sprintf(txtkey, "%s", pd->szEdName);
						bEnt->GetKV(pd->szKey, txtval, 256);
						tmparrdata.push_back(KeyVal(pd->szKey, txtval));
					}
				}
			}

			tmparr.clear();

			for (int i = 0; i < pt->numFields; ++i)
			{
				pd = &pt->pData[i];
				if (pd->szKey && pd->szEdName && !(pd->flags & PDFF_NOEDIT) && pd->editor.type != PDE_NONE)
				{
					sprintf(txtkey, "%s", pd->szEdName);
					bEnt->GetKV(pd->szKey, txtval, 256);
					tmparrdata.push_back(KeyVal(pd->szKey, txtval));
				}
			}

			char txt[256];
			SXLevelEditor::ComboBoxGameClass->GetItemText(SXLevelEditor::ComboBoxGameClass->GetSel(), txt);
			SXGame_RemoveEntity(bEnt);

			bEnt = SXGame_CreateEntity(txt);
			bEnt->SetFlags(bEnt->GetFlags() | EF_EXPORT | EF_LEVEL);


			for (int i = 0; i < tmparrdata.size(); ++i)
			{
				bEnt->SetKV(tmparrdata[i].key.c_str(), tmparrdata[i].val.c_str());
			}

			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
			sel = -1;
			for (int i = 0; i < SXLevelEditor::ListBoxList->GetCountItem(); ++i)
			{
				if (SXLevelEditor::ListBoxList->GetItemData(i) == bEnt->GetId())
				{
					sel = i;
					break;
				}
			}

			if (sel < 0)
			{

			}

			SXLevelEditor::ListBoxList->SetSel(sel);
			SXLevelEditor::GameSel(sel);
		}
		else if (SXLevelEditor::ComboBoxGreenSel->GetHWND() == handle_elem)
		{
			int sel = SXLevelEditor::ComboBoxGreenSel->GetSel();
			GData::Editors::GreenRenderBox = false;
			
			if (sel == 0)
			{
				SXLevelEditor::StaticGreenSelX->SetText("Position X:");
				SXLevelEditor::StaticGreenSelY->SetText("Position Y:");
				SXLevelEditor::StaticGreenSelZ->SetText("Position Z:");
				SXLevelEditor::EditGreenSelX->SetText("");
				SXLevelEditor::EditGreenSelY->SetText("");
				SXLevelEditor::EditGreenSelZ->SetText("");
			}
			else if (sel == 2)
			{
				GData::Editors::GreenRenderBox = true;

				SXLevelEditor::StaticGreenSelX->SetText("Width volume:");
				SXLevelEditor::StaticGreenSelY->SetText("Height volume:");
				SXLevelEditor::StaticGreenSelZ->SetText("Depth volume:");
				SXLevelEditor::EditGreenSelX->SetText(String(GData::Editors::GreenBoxWHD.x).c_str());
				SXLevelEditor::EditGreenSelY->SetText(String(GData::Editors::GreenBoxWHD.y).c_str());
				SXLevelEditor::EditGreenSelZ->SetText(String(GData::Editors::GreenBoxWHD.z).c_str());
			}

			
		}
	}

	return 0;
}

LRESULT SXLevelEditor_GroupBoxList_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == LBN_SELCHANGE)
	{
		if (handle_elem == SXLevelEditor::ListBoxList->GetHWND())
			SXLevelEditor_ListBoxList_Click(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void SXLevelEditor_Transform(DWORD timeDelta)
{
	static bool IsSFirstLBMTransform = false;
	static bool IsSFirstRBMTransform = false;
	static int CoordinateTransformation = -1;

	if (SXLevelEditor::RadioButtonGeomPosX->GetCheck() || SXLevelEditor::RadioButtonGeomRotX->GetCheck() || SXLevelEditor::RadioButtonGeomScaleX->GetCheck() || SXLevelEditor::RadioButtonGreenSelX->GetCheck())
		CoordinateTransformation = 0;
	else if (SXLevelEditor::RadioButtonGeomPosY->GetCheck() || SXLevelEditor::RadioButtonGeomRotY->GetCheck() || SXLevelEditor::RadioButtonGeomScaleY->GetCheck() || SXLevelEditor::RadioButtonGreenSelY->GetCheck())
		CoordinateTransformation = 1;
	else if (SXLevelEditor::RadioButtonGeomPosZ->GetCheck() || SXLevelEditor::RadioButtonGeomRotZ->GetCheck() || SXLevelEditor::RadioButtonGeomScaleZ->GetCheck() || SXLevelEditor::RadioButtonGreenSelZ->GetCheck())
		CoordinateTransformation = 2;

	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGeom_ModelsGetCount() > 0)
		{
			DWORD selmodel = SXLevelEditor::ListBoxList->GetSel();
			if (SXLevelEditor::RadioButtonGeomPosX->GetCheck() || SXLevelEditor::RadioButtonGeomPosY->GetCheck() || SXLevelEditor::RadioButtonGeomPosZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetPosition(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
					{
						RECT rc;
						GetWindowRect(GetForegroundWindow(), &rc);
						UINT cx = (rc.right + rc.left) / 2;
						UINT cy = (rc.bottom + rc.top) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditGeomPosX->SetText(tmpPosX);
				SXLevelEditor::EditGeomPosY->SetText(tmpPosY);
				SXLevelEditor::EditGeomPosZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonGeomRotX->GetCheck() || SXLevelEditor::RadioButtonGeomRotY->GetCheck() || SXLevelEditor::RadioButtonGeomRotZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetRotation(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
					{
						RECT rc;
						GetWindowRect(GetForegroundWindow(), &rc);
						UINT cx = (rc.right + rc.left) / 2;
						UINT cy = (rc.bottom + rc.top) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditGeomRotX->SetText(tmpPosX);
				SXLevelEditor::EditGeomRotY->SetText(tmpPosY);
				SXLevelEditor::EditGeomRotZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonGeomScaleX->GetCheck() || SXLevelEditor::RadioButtonGeomScaleY->GetCheck() || SXLevelEditor::RadioButtonGeomScaleZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetScale(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
					{
						RECT rc;
						GetWindowRect(GetForegroundWindow(), &rc);
						UINT cx = (rc.right + rc.left) / 2;
						UINT cy = (rc.bottom + rc.top) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditGeomScaleX->SetText(tmpPosX);
				SXLevelEditor::EditGeomScaleY->SetText(tmpPosY);
				SXLevelEditor::EditGeomScaleZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}
		}
	}
	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN &&
			(
			(SXLevelEditor::ComboBoxGreenSel->GetSel() == 0 && GData::Editors::ActiveElement >= 0 && GData::Editors::ActiveGreenSplit >= 0 && GData::Editors::ActiveGreenObject >= 0) ||
			(SXLevelEditor::ComboBoxGreenSel->GetSel() == 2)
			)
		)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGeom_GreenGetCount() > 0)
		{
			DWORD selmodel = SXLevelEditor::ListBoxList->GetSel();
			if (SXLevelEditor::RadioButtonGreenSelX->GetCheck() || SXLevelEditor::RadioButtonGreenSelY->GetCheck() || SXLevelEditor::RadioButtonGreenSelZ->GetCheck())
			{
				float3 pos;
				if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 2)
					pos = GData::Editors::GreenBoxWHD;
				else
				{
					float3_t pos2;
					SGeom_GreenGetPosObject(GData::Editors::ActiveElement, GData::Editors::ActiveGreenSplit, GData::Editors::ActiveGreenObject, &pos2);
					pos = pos2;
				}

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += float(timeDelta) * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= float(timeDelta) * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
					{
						RECT rc;
						GetWindowRect(GetForegroundWindow(), &rc);
						UINT cx = (rc.right + rc.left) / 2;
						UINT cy = (rc.bottom + rc.top) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += float(timeDelta) * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditGreenSelX->SetText(tmpPosX);
				SXLevelEditor::EditGreenSelY->SetText(tmpPosY);
				SXLevelEditor::EditGreenSelZ->SetText(tmpPosZ);

				if (SXLevelEditor::ComboBoxGreenSel->GetSel() == 2)
					GData::Editors::GreenBoxWHD = pos;
				else
					SGeom_GreenSetPosObject(GData::Editors::ActiveElement, &GData::Editors::ActiveGreenSplit, &GData::Editors::ActiveGreenObject, &float3_t(pos));
			}
		}
	}

	else if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT))
		{
			float posy = 0.f;

			if (SSInput_GetKeyState(SIK_UP))
				posy += float(timeDelta) * 0.001f;
			if (SSInput_GetKeyState(SIK_DOWN))
				posy -= float(timeDelta) * 0.001f;

			if (SSInput_GetKeyState(SIM_LBUTTON))
			{
				if (IsSFirstRBMTransform)
				{
					RECT rc;
					GetWindowRect(GetForegroundWindow(), &rc);
					UINT cx = (rc.right + rc.left) / 2;
					UINT cy = (rc.bottom + rc.top) / 2;
					POINT p;
					GetCursorPos(&p);
					POINT centr;
					centr.x = cx; centr.y = cy;

					if (cy != UINT(p.y))
						posy += float(timeDelta) * 0.001f * float(-int(p.y - cy));
				}
				else
					IsSFirstRBMTransform = true;
				CameraUpdate::CentererCursor();
			}
			else
			{
				IsSFirstLBMTransform = false;
				IsSFirstRBMTransform = false;
			}

			SAIG_QuadSelectedAddPosY(posy);
		}
	}
}

void SXLevelEditor::FinalImageUncheckedMenu()
{
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, false);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_NORMALS, false);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_PARAMETERS, false);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_SPECULAR, false);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	SXLevelEditor::CheckBoxTBRColor->SetCheck(false);
	SXLevelEditor::CheckBoxTBRNormal->SetCheck(false);
	SXLevelEditor::CheckBoxTBRParam->SetCheck(false);
	SXLevelEditor::CheckBoxTBRAmDiff->SetCheck(false);
	SXLevelEditor::CheckBoxTBRSpecular->SetCheck(false);
	SXLevelEditor::CheckBoxTBRLighting->SetCheck(false);
}