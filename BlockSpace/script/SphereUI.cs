using UnityEngine;
using System.Collections;
using System.Collections.Generic;
// --------------------------------------------------
// 合成器
// --------------------------------------------------
public class SphereUI : MonoBehaviour {
    // 模板
    public GameObject bbtemplate;
    public GameObject defaultmeshobj;
    public GameObject line3dtemplate;
    public GameObject productbox;

    // UI特效
    ArrayList bblist    = new ArrayList();
    // 激光
    ArrayList linelist = new ArrayList();
    // 模型上的坐标点
    Vector3[] targets1;
    // time
    float gtime = 0.0f;
    // 颜色
    Dictionary<string, Color> scors = new Dictionary<string, Color>();
    // 文字
    string[] itemtxts = { "房间", "窗户", "大门", "床", "台灯", "墙3", "墙4", "墙5" };
    // --------------------------------------------------
    void Start () {        
		bbtemplate.SetActive(false);
        defaultmeshobj.SetActive(false);
        line3dtemplate.SetActive(false);
    }
    // --------------------------------------------------
    void openbox()
    {
        closebox();
        if (PMDLL.createMesh(this.gameObject, 1, 8))
        {
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;

            // 挂点
            int cnt = PMDLL.gettargetcnt();
            targets1 = new Vector3[cnt];
            PMDLL.getTargets(targets1);
        }    
         
        for (int i = 0; i < itemtxts.Length; i++)
        {
            GameObject q = Instantiate(bbtemplate);
            q.name = "itm" + (1 + i).ToString();
            // 色彩    
            scors.Add(q.name, new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 0.5f));
            {// 文本
                GameObject t = q.transform.GetChild(0).gameObject;
                t.SetActive(true);

                TextMesh tm = t.GetComponent<TextMesh>();
                tm.text = itemtxts[i];
                tm.anchor = TextAnchor.MiddleLeft;
                t.transform.position = new Vector3((tm.text.Length * tm.characterSize * 0.05f), 0, -0.01f) / 4;
            }
            q.SetActive(true);
            q.transform.SetParent(transform);
            bblist.Add(q);
        }
    }
    void closebox()
    {
        for (int i = 0; i < bblist.Count; i++)
        {
            GameObject go = (GameObject)bblist[i];
            GameObject.Destroy(go);
        }
        bblist.Clear();
        for (int i = 0; i < linelist.Count; i++)
        {
            GameObject go = (GameObject)linelist[i];
            GameObject.Destroy(go);
        }
        linelist.Clear();
        scors.Clear();
        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
        mc.sharedMesh = defaultmeshobj.GetComponent<MeshFilter>().mesh;
        this.gameObject.GetComponent<MeshFilter>().mesh = mc.sharedMesh;
    }
    
    // --------------------------------------------------
    void Update () {
        gtime += Time.deltaTime;
        float dang = 0.1F * gtime;
        float r = 1.0f;
        //float rr = 1.0f + 0.5f * Mathf.SmoothStep(-1, 1, 0.5F + 0.5F * Mathf.Sin(gtime * 10) );
        for (int i = 0; i < bblist.Count; i++)
        {
            float ang = i * 2 * Mathf.PI / bblist.Count + dang;            
            GameObject go = (GameObject)bblist[i];
            go.transform.position = transform.position + new Vector3(r * Mathf.Cos(ang), 0.5f, r * Mathf.Sin(ang));
            go.transform.localScale = new Vector3(0.5f, 0.5f, -1);
            go.transform.LookAt(Camera.main.transform.position, Vector3.up);
        }        
    }
    // --------------------------------------------------
    // GUI
    // --------------------------------------------------
    // 触发事件
    void TriggerEvent(GameObject go)
    {
        if (linelist.Count == 0)
        {// 开启激光
            for (int i = 0; i < targets1.Length; i++)
            {
                GameObject line = (GameObject)Instantiate(line3dtemplate);
                ((Line3d)line.GetComponent<Line3d>()).setLine(go.transform.parent.position + targets1[i], productbox.transform.position);
                line.SetActive(true);
                linelist.Add(line);
            }
        }
        {// 改变生成物的颜色
            Material mat = ((Renderer)productbox.GetComponent<Renderer>()).material;
            
            mat.SetColor("_TintColor", scors[go.name]);
            Debug.Log("scors[go.name] : " + scors[go.name].ToString());
        }
        Debug.Log("TriggerEvent: " + go.name);
    }
    // --------------------------------------------------
    void OnGUI()
    {
        Event e = Event.current;
        if (e.isMouse)
        {
            // pick	
            if (Input.GetMouseButtonDown(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject == gameObject || hit.collider.gameObject.transform.parent == gameObject.transform) )
                {
                    //Debug.Log("picked 3d ui: " + hit.collider.gameObject.GetInstanceID().ToString() + hit.collider.gameObject.ToString());
                    if (hit.collider.gameObject.name.Contains("OperBox"))
                    {// 打开盒子
                        openbox();
                        Debug.Log("picked OperBox: " + hit.collider.gameObject.ToString());
                    }
                    else if (hit.collider.gameObject.name.Contains("itm"))
                    {// 操作
                        TriggerEvent(hit.collider.gameObject);
                      //  Debug.Log("picked itm: " + hit.collider.gameObject.ToString());
                    }
                }
              else
              {
                  closebox();
              }
            }
        }
    }
    // --------------------------------------------------
    void OnMouseDrag()
    {

    }
}
