using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using System.IO;

public class StartSceneUI : MonoBehaviour {

    public GUIStyle style1;
    public GUIStyle style2;
    private AsyncOperation async = null;
    bool loading = false;
    // Use this for initialization
    void Start () {
        
    }	
	// Update is called once per frame
	void Update () {
	
	}
    public IEnumerator loadScene()
    {
        loading = true;
        async = SceneManager.LoadSceneAsync("Main");
        yield return async;

        Debug.Log("Complete!");
        loading = false;
    }
    void OnGUI()
    {
        GUI.skin.button = style1;
        int nwid = (int)(Screen.width / 160) - 1;
        int nhgt = (int)(Screen.height / 256) - 1;
        int dw = Screen.width - nwid * 160;
        int dh = Screen.height - nhgt * 256;
        for (int i = 0; i < nwid; i++)
        for (int j = 0; j < nhgt; j++)
        {
            string sceneName = "s" + (j * nwid + i).ToString();
            string filepath = Application.dataPath + @"/StreamingAssets/" + sceneName;
            if (!Directory.Exists(filepath))
                GUI.skin.button = style1;
            else
                GUI.skin.button = style2;

            if (GUI.Button(new Rect(dw / 2 + i * 160, dh / 2 + j * 256, 160, 256), ""))
            {
                if (!loading)
                {
                    Common.sceneName = sceneName;
                    StartCoroutine("loadScene");
                }
            }
        }
    }
}
