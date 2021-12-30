using System.IO;
using UnityEngine;
using System.Collections;
public class starteffects : MonoBehaviour {
    public GameObject GODCUBE;
    public GameObject STARPS;
    public GameObject MOON;
    public GameObject PORTALBOX;
    public int framecnt = 10000;
    float Exposure = 0;
    float moonspeed = 0.00001f;
    // Use this for initialization
    void Start () {
        RenderSettings.skybox.SetFloat("_Exposure", 0);
        
        STARPS.SetActive(false);
        MOON.SetActive(false);
        freemod.movementlocked = true;
    }
    void lightnow()
    {
        Exposure = Mathf.Lerp(Exposure, 0.85f, 0.025f);
        RenderSettings.skybox.SetFloat("_Exposure", Exposure);
    }
    void Creating()
    {
        float y = Mathf.Lerp(GODCUBE.transform.position.y, 0, 0.1f);
        GODCUBE.transform.position = new Vector3(GODCUBE.transform.position.x, y, GODCUBE.transform.position.z);
    }
    
    void Moving2Moon1()
    {        
        moonspeed = Mathf.Lerp(moonspeed, 0.0025f, 0.01f);

        MOON.transform.position = Vector3.Lerp(MOON.transform.position, Vector3.zero + Vector3.forward * 80 + Vector3.up * (100 / 2), moonspeed);
        MOON.transform.localScale = Vector3.Lerp(MOON.transform.localScale, Vector3.one * 100, moonspeed);
    }
    void Moving2Moon2()
    {
        MOON.transform.position = Vector3.Lerp(MOON.transform.position, Vector3.zero + Vector3.forward * 80, 0.025f);
        MOON.transform.localScale = Vector3.Lerp(MOON.transform.localScale, Vector3.one * 217.12f, moonspeed);
    }
    void Moving2Moon3()
    {
        MOON.transform.position = Vector3.Lerp(MOON.transform.position, Vector3.zero, 0.025f);
        MOON.transform.localScale = Vector3.Lerp(MOON.transform.localScale, Vector3.one * 217.12f, moonspeed);
    }
    void rotation()
    {
        MOON.transform.Rotate(Vector3.up, 0.45f);
    }
    // Update is called once per frame
    void Update () {
        framecnt++;
        int framcnt0 = 0;
        {// 宇宙初始的空虚沉寂
            framcnt0 += 180;
            if (framecnt < framcnt0)
            {                
                return;
            }
        }
        {// 造物主触发大爆炸
            framcnt0 += 100;
            if (framecnt < framcnt0)
            {
                Creating();
                return;
            }
            else if (framecnt == framcnt0)
            {
                STARPS.SetActive(true);
                GODCUBE.SetActive(false);
                MOON.SetActive(true);
                return;
            }
        }
        {// 各类星球飞离宇宙中心
            framcnt0 += 400;
            if (framecnt < framcnt0)
            {               
                return;
            }
        }        
        {// 我们的星球出现
            framcnt0 += 780;
            if (framecnt < framcnt0)
            {
                Moving2Moon1();
                rotation();

                return;
            }
            if (framecnt == framcnt0 - 500)
            {
                STARPS.SetActive(false);
                return;
            }
        }
        {// 表面漫游
            framcnt0 += 250;
            if (framecnt < framcnt0)
            {
                Moving2Moon2();
                rotation();                
                return;
            }
            else if (framecnt == framcnt0)
            {                
                return;
            }
        }
        {// 天空闪亮了
            framcnt0 += 300;
            if (framecnt < framcnt0)
            {
                Moving2Moon3();
                lightnow();
                return;
            }
            else if (framecnt == framcnt0)
            {
                STARPS.SetActive(false);
                return;
            }
        }

        {// 确定最终数值
            GODCUBE.SetActive(false);
            STARPS.SetActive(false);
            MOON.SetActive(true);
            PORTALBOX.SetActive(true);

            MOON.transform.localScale = Vector3.one * 217.12f;
            MOON.transform.position = Vector3.zero;
            RenderSettings.skybox.SetFloat("_Exposure", 0.85f);
            freemod.movementlocked = false;
            MOON.GetComponent<MeshCollider>().enabled = true;
        }       
    }
}
