using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
public class StartScene : MonoBehaviour {

    private AsyncOperation async = null;
    // Use this for initialization
    void Start () {
	
	}
    //异步加载
    public IEnumerator loadScene()
    {        
        async = SceneManager.LoadSceneAsync("Main");
        yield return async;

        Debug.Log("Complete!");
    }

    public void OnClick()
    {
        Debug.Log("Button");
        StartCoroutine("loadScene");        
    }
}
