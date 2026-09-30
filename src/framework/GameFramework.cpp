#pragma warning(disable:4995)
#include "GameFramework.h"
#include <strsafe.h>
#include <math.h>
#include <direct.h>
#include <malloc.h>
#include "../../third_party/fmod/api/inc/fmod.h"
#pragma comment(lib,"third_party/fmod/api/lib/fmodvc.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


#pragma warning(disable:4244)

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL; // Buffer to hold tea2
bool fullscreen;

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
	float u,v;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)




//-----------------------------------------------------------------------------
// Name: Text Renderer
//-----------------------------------------------------------------------------
LPD3DXFONT TextRenderer::FindFont(int size)
{
	std::map<int, LPD3DXFONT>::iterator fit=fonts.find(size);
	if (fit==fonts.end())
	{
		LPD3DXFONT m_pFont=NULL;
		D3DXCreateFont(g_pd3dDevice,size,0,FW_NORMAL,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Lucida Console"), &m_pFont );
		fit=fonts.insert(std::pair<int,LPD3DXFONT>(size,m_pFont)).first;
	}
	if (fit!=fonts.end())
	{
		return fit->second;
	}
	else return NULL;
}

void TextRenderer::ReleaseFonts()
{
	if (intextbatch) EndTextBatch();
	for (std::map<int, LPD3DXFONT>::iterator it=fonts.begin();it!=fonts.end();++it)
	{
		MA_RELEASE(it->second);
	}
	fonts.clear();
	MA_RELEASE(fontsprite);
	intextbatch=0;
}

void TextRenderer::StartTextBatch(int size)
{
	if (intextbatch) EndTextBatch();
	intextbatch=1;
	if (size) FindFont(size);
	if (!fontsprite) 
	{
		D3DXCreateSprite( g_pd3dDevice, &fontsprite);
	}
	if (fontsprite)
	{
		fontsprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	}
}

void TextRenderer::EndTextBatch()
{
	if (intextbatch)
	{
		intextbatch=0;
		if (fontsprite)
		{			
			fontsprite->End();
		}
		//for (std::map<int, LPD3DXFONT>::iterator fit=fonts.begin();fit!=fonts.end();++fit) fit->second->End();		
	}
}

int TextRenderer::DrawSomeText(int x, int y, int size, int col, bool centered, const char *pFormat, ...)
{
	char debugText[8192];
	va_list	parameter;
	va_start(parameter,pFormat);
	vsprintf(debugText,pFormat,parameter);

	LPD3DXFONT m_pFont=FindFont(size);
	if (!m_pFont) return 0;
	RECT r={x,y,x+100,y+100};	
	
	m_pFont->DrawText(intextbatch?fontsprite:NULL,debugText,-1,&r,DT_CALCRECT,0);
	int wid=r.right-r.left;
	if (centered) r.left-=wid/2;
	if (centered) r.right-=wid/2;
	m_pFont->DrawText(intextbatch?fontsprite:NULL,debugText,-1,&r,DT_TOP|DT_LEFT,col);
	lasttextheight=r.bottom-r.top;
	lasttextwidth=r.right-r.left;
	return lasttextheight;
}




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = !fullscreen;
	d3dpp.SwapEffect = fullscreen? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE ;
	d3dpp.BackBufferFormat = fullscreen ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth=800;
	d3dpp.BackBufferHeight=600;
	d3dpp.FullScreen_RefreshRateInHz = fullscreen? 60 : 0;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our tea2. The vertex
//       buffer is basically just a chuck of memory that holds tea2. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{
  
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
                                                  D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pVB != NULL )        
        g_pVB->Release();

    if( g_pd3dDevice != NULL ) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )       
        g_pD3D->Release();

	
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------

bool g_keydown[256];
int g_keyhit[256];
int g_mb;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		g_mb|=1;
		g_keydown[VK_LBUTTON]=true;
		g_keyhit[VK_LBUTTON]++;
		break;
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		g_keydown[VK_RBUTTON]=true;
		g_keyhit[VK_RBUTTON]++;
		g_mb|=2;
		break;
	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		g_mb|=4;
		g_keydown[VK_MBUTTON]=true;
		g_keyhit[VK_MBUTTON]++;
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		g_mb&=~1;
		g_keydown[VK_LBUTTON]=false;
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		g_mb&=~2;
		g_keydown[VK_RBUTTON]=false;
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		g_mb&=~4;
		g_keydown[VK_MBUTTON]=false;
		break;
		
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		g_keydown[wParam&255]=true;
		g_keyhit[wParam&255]++;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		g_keydown[wParam&127]=false;
		break;

    case WM_DESTROY:
        Cleanup();
        PostQuitMessage( 0 );
        return 0;
	case WM_ACTIVATEAPP:
		if (!wParam)
		{
			memset(g_keydown,0,sizeof(g_keydown));
		}
		break;

	case WM_ACTIVATE:
		if( WA_INACTIVE != wParam )
		{
			// Make sure the device is acquired, if we are gaining focus.
			
		}
		break;
    }


    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------

LARGE_INTEGER starttime;
LARGE_INTEGER freq;
extern HWND hWnd;
HWND hWnd;
Window gameWindow;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT )
{
	/*int id = MessageBox(NULL,"Do you want to open fullscreen mode?", gameWindow.name,MB_YESNOCANCEL);
	if (id==IDCANCEL) return 0;
	fullscreen=(id==IDYES);*/
	fullscreen = 1;

    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "SpaceInvaders", NULL };
    RegisterClassEx( &wc );

	RECT r={0,0,gameWindow.width,gameWindow.height};
	int style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	style|=WS_VISIBLE;
	AdjustWindowRect(&r,style,false);

    // Create the application's window
     hWnd = CreateWindow(	"SpaceInvaders", gameWindow.name,
							style, 0,0,r.right-r.left,r.bottom-r.top,
                            GetDesktopWindow(), NULL, wc.hInstance, NULL );
	
	 FSOUND_Init(44100, 42, 0);

	QueryPerformanceCounter(&starttime);
	QueryPerformanceFrequency(&freq);

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        // Create the vertex buffer
        if( SUCCEEDED( InitVB() ) )
        {
			SetCursor(LoadCursor(NULL,IDC_ARROW));

            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );
			
            Game();
        }
    }

    UnregisterClass( "SpaceInvaders", wc.hInstance );
    return 0;
}

void GetMousePos(float& x, float& y) // 0,0 is top left; 800,600 is bottom right
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	x = p.x;
	y = p.y;
}

bool IsKeyDown(int key) // use windows VK_ codes for special keys, eg VK_LEFT; use capital chars for letter keys eg 'A', '0'
{
	return g_keydown[key & 255];
}

bool WantQuit(DWORD clearcol)
{
	// Enter the message loop
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		if (msg.message == WM_QUIT) return true;
	}	

	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, clearcol, 1.0f, 0 );
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE,false);

	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);

	g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);

	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING,false);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	D3DVIEWPORT9 vp={ 0,0, 800, 600, 0, 1};

	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	// Begin the scene
	g_pd3dDevice->BeginScene() ;

	return false;
}
int	GetTimeInMS() // ...since start of program
{
	LARGE_INTEGER arse;
	QueryPerformanceCounter(&arse);
	return ((arse.QuadPart-starttime.QuadPart)*1000) / freq.QuadPart;
}

void Flip()
{
	
	static int lastflip=0;
	if (lastflip==0) lastflip = GetTimeInMS();
	g_pd3dDevice->EndScene();

	// Present the backbuffer contents to the display
	
	//while (GetTimeInMS()<lastflip+1000/60) ;// Sleep(0); // clamp to max of 60hz
	lastflip=GetTimeInMS();

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	Sleep(0);
	memset(g_keyhit,0,sizeof(g_keyhit));
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	
}




//-----------------------------------------------------------------------------
// Name: Graphic library
//-----------------------------------------------------------------------------

void* LoadSprite(const char *fname)
{
	IDirect3DTexture9 *tex = NULL;
	D3DXCreateTextureFromFile(g_pd3dDevice,fname,&tex);
	return tex;
}

void SetCurrentTexture(void *tex )
{
	IDirect3DTexture9 *t = (IDirect3DTexture9 *)tex;
	g_pd3dDevice->SetTexture(0,t);
}

void DrawSprite(void *sprite, float xcentre, float ycentre, float halfWidth, float halfHeight, float rotate_angle_radians, DWORD col )
{
	SetCurrentTexture(sprite);
	float c=cosf(rotate_angle_radians);
	float s=sinf(rotate_angle_radians);
#define ROTATE(xx,yy) xcentre+(xx)*c+(yy)*s,ycentre+(yy)*c-(xx)*s 
	CUSTOMVERTEX tea2[] =
	{
		{ ROTATE(-halfWidth,-halfHeight), 0.5f, 1.0f, col, 0,0, }, // x, y, z, rhw, color
		{ ROTATE(halfWidth,-halfHeight), 0.5f, 1.0f, col, 1,0, },
		{ ROTATE(-halfWidth, halfHeight), 0.5f, 1.0f, col, 0,1, },
		{ ROTATE(halfWidth, halfHeight), 0.5f, 1.0f, col, 1,1, },
	};
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, tea2, sizeof(CUSTOMVERTEX));
}

void DrawLine(float x1, float y1, float x2, float y2, DWORD col ) // no texture
{
	IDirect3DBaseTexture9 *oldtex = NULL;
	g_pd3dDevice->GetTexture(0, &oldtex);
	g_pd3dDevice->SetTexture(0, NULL);
	CUSTOMVERTEX tea2[] =
	{
		{ x1, y1, 0.5f, 1.0f, col, 0,0, }, // x, y, z, rhw, color
		{ x2, y2, 0.5f, 1.0f, col, 1,0, }
	};
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, tea2, sizeof(CUSTOMVERTEX));

	g_pd3dDevice->SetTexture(0, oldtex);
}




//-----------------------------------------------------------------------------
// Name: FMOD
//-----------------------------------------------------------------------------

FSOUND_STREAM *music;

int PlayMusic(const char *fname, float volume)
{
	if (music) StopMusic();
	music = FSOUND_Stream_Open(fname,FSOUND_LOOP_NORMAL,0,0);
	int chan = FSOUND_Stream_Play(FSOUND_FREE,music);
	if (volume<=0) volume=0;
	if (volume>1) volume=1;
	FSOUND_SetVolume(chan, (int)(volume*255));
	return chan;
}

void StopMusic()
{
	if (music)
	{
		FSOUND_Stream_Close(music);
	}
	music=NULL;
}

void *LoadSnd(const char *fname, bool looped)
{
	int flags=0;
	if (looped) flags|=FSOUND_LOOP_NORMAL;
	return FSOUND_Sample_Load(FSOUND_FREE,fname,flags,0,0);
}

int PlaySnd(void *sound, float volume)
{
	if (!sound) return -1;
	if (volume<=0) volume=0;
	if (volume>1) volume=1;
	int chan = FSOUND_PlaySound(FSOUND_FREE,(FSOUND_SAMPLE*)sound);
	FSOUND_SetVolume(chan, (int)(volume*255));
	return chan;
}

void StopSnd(int handle)
{
	if (handle<=0) return ;
	FSOUND_StopSound(handle);
}

void ChangeVolume(int handle, float volume)
{
	if (handle<=0) return ;
	if (volume<=0) volume=0;
	if (volume>1) volume=1;
	FSOUND_SetVolume(handle, (int)(volume*255));
}
