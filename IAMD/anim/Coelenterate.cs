using UnityEngine;
using System.Collections;
/// <summary>
/// 动物蒙皮
/// </summary>
public class Coelenterate : MonoBehaviour
{
    public GameObject rootbone;
    public Vector3[] bones;
    public float[] bonescls;
    float gtime = 0;
    bool bvis = true;
    string roottag = "";
    // -------------------------------------------------------
    void setSpineRoot(GameObject root)
    {
        rootbone = root;
        transform.position = root.transform.position;
        transform.rotation = root.transform.rotation;
        roottag = "spine";
        updatebones(rootbone);
    }
    void setLimbRoot(GameObject root)
    {
        rootbone = root;
        transform.position = root.transform.position;
        transform.rotation = root.transform.rotation;
        roottag = "limb";
        updatebones(rootbone);
    }
    void Update()
    {
        if (!bvis)
            return;
        if (gtime >= 10)
            gtime = 0;
        gtime += Time.deltaTime;//Mathf.Abs(gtime - 5) / 5
        if(rootbone)
           updatebones(rootbone);
        if (bones != null)
        {
            
        }
    }
    // -------------------------------------------------------
    void updatebones(GameObject boneRoot)
    {
        if (!boneRoot)
            return;
        GameObject b = boneRoot;
        int cnt = 1;
        while (b && b.transform.childCount > 0)
        {
            b = b.transform.GetChild(0).gameObject; // 骨骼是第一个挂点
            cnt++;
        }
        bones = new Vector3[cnt];
        bonescls = new float[cnt];

        int ind = 0;
        b = boneRoot;
        while (b)
        {
            bones[ind] = transform.InverseTransformPoint(b.transform.position) * 0.1f;
            bonescls[ind] = b.transform.localScale.x;
            ind++;

            if (b.transform.childCount > 0)
                b = b.transform.GetChild(0).gameObject;
            else
                break;
        }
        //transform.position = boneRoot.transform.position;
        //transform.rotation = boneRoot.transform.rotation;
    }
    // -------------------------------------------------------
    void OnGUI()
    {
        if (roottag == "spine")
        {
            if (GUI.Button(new Rect(Screen.width - 128, Screen.height / 2, 64, 64), bvis ? "bones" : "skin"))
            {
                bvis = !bvis;                
                transform.parent.gameObject.SendMessage("onskinvis", bvis);

                gameObject.GetComponent<MeshRenderer>().enabled = bvis;
                GameObject b = rootbone;
                int ind = 0;
                while (b)
                {
                    b.GetComponent<MeshRenderer>().enabled = !bvis;
                    ind++;

                    if (b.transform.childCount > 0)
                        b = b.transform.GetChild(0).gameObject;
                    else
                        break;
                }
            }
        }
        Event e = Event.current;
        if (e.isMouse)
        {
            if (Input.GetMouseButtonUp(0))
            {
                updatebones(rootbone);
            }
        }
    }
}