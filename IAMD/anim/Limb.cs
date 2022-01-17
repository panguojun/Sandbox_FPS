using UnityEngine;
using System.Collections;

public class Limb : MonoBehaviour
{
    public GameObject Robot;
    public GameObject BONENODE;
    public GameObject Body;
    public Vector3[] bones;
    public float[] bonescls;

    GameObject rootbone;

    const float c_size = 0.5f;
    const int bodybones = 4;
    const int tailbones = 8;
    float gtime;

    // -------------------------------------------------------------------------------------
    void Setup(GameObject robot)
    {
        Robot = robot;
        setup(); 
        gtime = Time.time;
    }
    // -------------------------------------------------------------------------------------
    GameObject addbone(GameObject lnb, GameObject template = null)
    {
        if (template == null)
            template = BONENODE;
        GameObject nb = GameObject.Instantiate(template);
        nb.SetActive(true);
        nb.transform.parent = lnb.transform;
        nb.transform.position = lnb.transform.position;
        nb.transform.rotation = lnb.transform.rotation;
        return nb;
    }
    void setup()
    {        
        int numbones = 3;
        GameObject lnb = gameObject;
        for (int i = 0; i < numbones; i++)
        {
            GameObject nb = addbone(lnb);
            
            {// 肢骨
                if (i == 0)
                {// 上
                    nb.transform.position -= lnb.transform.up * (c_size);
                    nb.transform.localScale = Vector3.one;
                }
                else if(i == 1)
                {// 关节
                    nb.transform.Rotate(-30, 0, 0);
                    nb.transform.position -= lnb.transform.up * (c_size);
                    nb.transform.localScale = Vector3.one * 2;
                }
                else
                {// 下
                    nb.transform.position -= lnb.transform.up * (c_size);
                    nb.transform.localScale = Vector3.one * 1;
                }                
            }
            lnb = nb;
        }        
        GameObject body = GameObject.Instantiate(Body);
        body.transform.SetParent(transform);
        body.SetActive(true);
        body.SendMessage("setLimbRoot", gameObject);
    }   
}