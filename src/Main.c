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
		

	const signed int absx = PS4_Controller_Abs(&ps4c,PS4_CONTROLLER_LX);
	const signed int absy = PS4_Controller_Abs(&ps4c,PS4_CONTROLLER_LY);
	
	if(Stroke(ALX_KEY_A).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_UP).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_PAGEUP).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ -1.0f,0.0f },w->ElapsedTime));
	else if(Stroke(ALX_KEY_D).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_DOWN).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_PAGEDOWN).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 1.0f,0.0f },w->ElapsedTime));
	else if(absx >= 0 && absx < 128)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ F32_Map(absx,0.0f,255.0f,-1.0f,1.0f),0.0f },w->ElapsedTime));
	else if(absx >= 128)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ F32_Map(absx,0.0f,255.0f,-1.0f,1.0f),0.0f },w->ElapsedTime));
	
	if(Stroke(ALX_KEY_W).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_ESC).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_LALT).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,1.0f },w->ElapsedTime));
	else if(Stroke(ALX_KEY_S).DOWN || PPTX_Controller_Key(&pptx,PPTX_CONTROLLER_B).DOWN)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,1.0f },w->ElapsedTime));
	else if(absy >= 0 && absy < 128)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,F32_Map(absx,0.0f,255.0f,-1.0f,1.0f) },w->ElapsedTime));
	else if(absy >= 128)
		player.p = Vec2_Add(player.p,Vec2_Mulf((Vec2){ 0.0f,F32_Map(absx,0.0f,255.0f,-1.0f,1.0f) },w->ElapsedTime));

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