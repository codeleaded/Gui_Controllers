//#define IMAGE_PNG_ONLY

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/PS4_Controller.h"
#include "/home/codeleaded/System/Static/Library/PPTX_Controller.h"

Rect player;
TransformedView tv;
PS4_Controller ps4c;
PPTX_Controller pptx;

void Setup(AlxWindow* w){
	AlxFont_Resize(&window.AlxFont,16,16);

	player = Rect_New((Vec2){ 1.0f,1.0f },(Vec2){ 1.0f,2.0f });

	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	//TransformedView_Focus(&tv,&player.p,player.d);
	TransformedView_Zoom(&tv,(Vec2){ 0.5f,0.5f });

	pptx = PPTX_Controller_New("/dev/input/by-id/usb-XinChip_Wireless_Presenter_20221123001F-event-kbd");
	ps4c = PS4_Controller_New("/dev/input/by-id/usb-Sony_Interactive_Entertainment_Wireless_Controller-if03-event-joystick");
}
void Update(AlxWindow* w){
	PPTX_Controller_Update(&pptx);
	PS4_Controller_Update(&ps4c);
	
	TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

	if(PS4_Controller_Key(&ps4c,PS4_CONTROLLER_HOME).PRESSED){
		w->Running = 0;
	}
		

	if(Stroke(ALX_KEY_LEFT).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_UP).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_PAGEUP).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ -1.0f,0.0f },w->ElapsedTime));
	else if(Stroke(ALX_KEY_RIGHT).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_DOWN).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_PAGEDOWN).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 1.0f,0.0f },w->ElapsedTime));
	else
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ PS4_Controller_Dir(&ps4c,PS4_CONTROLLER_LX,0.2f,0),0.0f },w->ElapsedTime));
	
	if(Stroke(ALX_KEY_UP).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_ESC).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_LALT).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,-1.0f },w->ElapsedTime));
	else if(Stroke(ALX_KEY_DOWN).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_B).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,1.0f },w->ElapsedTime));
	else
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,PS4_Controller_Dir(&ps4c,PS4_CONTROLLER_LY,0.2f,0) },w->ElapsedTime));

	
	player.d = Vec2_Add(player.d,Vec2_Mulf((Vec2){ PS4_Controller_Dir(&ps4c,PS4_CONTROLLER_RX,0.2f,0),PS4_Controller_Dir(&ps4c,PS4_CONTROLLER_RY,0.2f,0) },w->ElapsedTime));
	player.d = Vec2_Clamp(player.d,(Vec2){ 0.5f,0.5f },(Vec2){ 10.0f,10.0f });

	Clear(LIGHT_BLUE);

	Rect_Render(WINDOW_STD_ARGS,TransformedView_WorldScreenRect(&tv,player),BLACK);

	String str = String_Format("O: %f,%f | Z: %f,%f",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
	String_Free(&str);
}
void Delete(AlxWindow* w){
	PS4_Controller_Free(&ps4c);
	PPTX_Controller_Free(&pptx);
}

int main(){
    if(Create("Controllers",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}