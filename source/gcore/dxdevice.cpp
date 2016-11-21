

int InitD3D(HWND hwnd, bool windowed, int width, int heigth, DWORD create_device_flags)
{
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		return SXGC_ERR_NON_DETECTED_D3D;
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	memset(&D3DAPP, 0, sizeof(D3DAPP));
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	D3DAPP.BackBufferCount = 1;
	D3DAPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3DAPP.MultiSampleQuality = 0;
	D3DAPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DAPP.hDeviceWindow = hwnd;
	D3DAPP.Windowed = windowed;
	D3DAPP.EnableAutoDepthStencil = true;
	D3DAPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	D3DAPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | create_device_flags | D3DCREATE_MULTITHREADED;
	D3DAPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	D3DAPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | create_device_flags | D3DCREATE_MULTITHREADED, &D3DAPP, &DXDevice)))
	{
		return SXGC_ERR_FAILED_INIT_D3D;
	}

	//SkyXEngine::Core::Data::Device->CreateQuery(D3DQUERYTYPE_EVENT , &SkyXEngine::Core::Data::D3DQueryEvent);

}