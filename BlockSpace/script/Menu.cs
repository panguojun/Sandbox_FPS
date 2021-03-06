using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Menu : MonoBehaviour {
	public GUIStyle guistyle;
	public GameObject bkimage;
    public GameObject howtoplay;
    public GameObject helppage;
    public GameObject helpfirstpage;
	public GameObject audioobj;
    public static bool bmusicswitch = true;
    public static bool beffectswitch = true;
	
    int menuid = 1;
	float lstesctime = 0;
    bool bingame = false;
    string lastscenename = "MainScene";
	bool bactive = false;
    // Use this for initialization
    void Start () {
        bingame = true;
    }
	void OnGUI () {
		if(bactive)
		{
			if(bkimage && !bkimage.activeSelf)
				bkimage.SetActive(true);
		}
		else
		{
			if(bkimage && bkimage.activeSelf)
				bkimage.SetActive(false);			
		}
		
		bactive = false;
		//if (Common.sceneName != "MainScene")
		//	return;
		Event e = Event.current;
		if (e.isKey) {
			if (Input.GetKeyDown (KeyCode.Escape)) {
				if (!bingame)
					return;
				if (Time.time - lstesctime < 0.5)
					return;
				lstesctime = Time.time;
				if (Common.bshowmenu) {
					Common.bshowmenu = false;
					Common.gamepaused = false;
					menuid = 2;
				} else {
					Common.bshowmenu = true;
					Common.gamepaused = true;
					menuid = 1;
				}
			}
		}
		if (!Common.bshowmenu)
		{
			return;
		}
		bactive = true;
		float size = 40 * 2;
        float ui_x = Screen.width / 2.0f - (size * 1.5f);
		float ui_y = Screen.height / 8;

		GUI.skin.button.fontSize = 50;
        {
            GUI.skin.button.normal.textColor = new Color(1.0f, 1.0f, 1.0f);
            GUI.contentColor = new Color(1.0f, 1.0f, 1.0f);
        }
        GUI.skin.button.normal.background = null;
        GUI.skin.button.active.background = null;
		//GUI.contentColor = Color.white;

        int lanid = Common.language == "en" ? 1 : 2;
		if (menuid == 1 && (bingame))
        {// 主菜单
            if (GUI.Button(new Rect(ui_x, ui_y, size * 3, size), lanid == 1 ? ("* How to play") : "* 游戏教程", guistyle))
            {
                Common.sceneName = "HelpScene";
                Common.lstmousedown = Time.time;
                helppage.SetActive(true);
                //helpfirstpage.SetActive(true);
                Common.bshowmenu = false;
                Common.gamepaused = true;
                bingame = true;
                return;
            }
            ui_y += size * 1.0f;
            if (GUI.Button(new Rect(ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Play") : "* 继续", guistyle))
            {
                Common.bshowmenu = false;
                Common.gamepaused = false;
                bingame = true;
            }
            
			ui_y += size * 1.0f;

            if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Settings") : "* 设置", guistyle)) {
				menuid = 2;
                return;
			}

			ui_y += size * 1.0f;

            if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* About") : "* 关于", guistyle)) {
				menuid = 3;
                return;
            }

			ui_y += size * 1.0f;

            if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Exit") : "* 退出", guistyle)) {
				Application.Quit ();
			}
		} else if (menuid == 2) {// 菜单

			if (Screen.fullScreen) {
                if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Windowed") : "* 窗口", guistyle)) {
					Screen.SetResolution (Screen.currentResolution.width, Screen.currentResolution.height, false);


				}
			} else {
                if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Fullscreen") : "* 全屏", guistyle)) {
					Screen.SetResolution (Screen.currentResolution.width, Screen.currentResolution.height, true);
				}
			}
            ui_y += size * 1.0f;
            if (GUI.Button(new Rect(ui_x, ui_y, size * 2, size), bmusicswitch ? (lanid == 1 ? "* Audio on" : "* 音乐 开") : (lanid == 1 ? "* Audio off" : "* 音乐 关"), guistyle))
            {
                bmusicswitch = !bmusicswitch;
				
				if(!audioobj.activeSelf)
					audioobj.SetActive(true);
				else
					audioobj.SetActive(false);
                return;
            }
           
            ui_y += size * 1.0f;
            if (GUI.Button(new Rect(ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Main menu") : "* 主菜单", guistyle))
            {
                menuid = 1;
                return;
            }

			ui_y += size * 1.0f;

            if (GUI.Button (new Rect (ui_x, ui_y, size * 2, size), lanid == 1 ? ("* Exit") : "* 退出", guistyle)) {
				Application.Quit ();
			}
		} 
		else if (menuid == 3) 
		{
			{
				//GUI.skin.label.alignment = TextAnchor.MiddleLeft;
				GUI.skin.label.fontSize = 40;
				GUI.skin.label.normal.textColor = Color.white;

                GUI.Label (new Rect (ui_x - 50, ui_y, 500, 128), (lanid == 1 ? ("Author: PanGuoJun") : ("作者：潘国俊")));
			}
			ui_y += size * 1.5f;
            if (GUI.Button (new Rect (ui_x + 50, ui_y, size * 2, size), lanid == 1 ? ("* Back") : ("* 返回"), guistyle)) {
                menuid = 1;
                return;
            }
		}
	}
}