using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class PackCube : MonoBehaviour
{
    public GameObject RAWCUBE;
    public GameObject CUBEMUD;    
    public GameObject IMAGECORE;

    // UI特效
    ArrayList itmlist = new ArrayList();
    // time
    float gtime = 0.0f;

    // 当前原料块
    GameObject selectedbox;
    // -------------------------------------------------------------
    // 玩法数值
    // -------------------------------------------------------------
    // 矿物数值
    static public Dictionary<string, int> minedat = new Dictionary<string, int>();
    // -------------------------------------------------------------
    // 初始化数值
    void setupdat()
    {
        {// 生成测试数值
            for (int i = 0; i < 50; i++)
            {
                string nm = ItemStaticData.sMineName[Random.Range(0, ItemStaticData.sMineName.Length)];
                minedat[nm] = Random.Range(1, 4);
            }
            minedat["菱锌矿"] = 2;
            minedat["萤石矿"] = 2;
            minedat["菱铁矿"] = 2;
            minedat["黄钾铁矾"] = 2;
            minedat["石灰岩"] = 2;
            minedat["白钨矿"] = 2;
        }
    }
    // -------------------------------------------------------------
    void addmine(string minename)
    {
        int n = minedat.ContainsKey(minename) ? minedat[minename] : 0;
        minedat[minename] = n + 1;
    }
    // -------------------------------------------------------------
    // 消耗矿物
    static public void usemine(string minename)
    {
        if(!minedat.ContainsKey(minename))
        {
            Common.debugerr("usemine 没找到此种矿物（" + minename + ")");
            return;
        }       
        int num = minedat[minename];
        if(num < 0)
        {
            Common.debugerr("usemine 数量不够 (" + minename + ")");
            return;
        }
        else if(num == 1)
        {
            minedat.Remove(minename);
            return;
        }       
        minedat[minename]--;        
    }
    // -------------------------------------------------------------
    // 打开盒子
    void openBox(int w, int h)
    {
        closebox();

        int sx = (w + 1) / 2;
        int sy = (h + 1) / 2;
        float usz = 0.1f;
        float deta = 0.01f;

        Dictionary<string, int>.Enumerator te = minedat.GetEnumerator();
       
        for (int j = sy; j >= -sy; j--)
        {
            for (int i = -sx; i <= sx; i++)
            {
                string curnm = "";
                int sz = 1;
                if (te.MoveNext())
                {
                    curnm = te.Current.Key;
                    sz = te.Current.Value;

                    for (int k = 0; k < sz; k++)
                    {
                        GameObject g = Instantiate(RAWCUBE);
                        //g.name = "itm" + i.ToString();
                        g.SetActive(true);
                        g.transform.localScale = new Vector3(usz, usz, usz);
                        g.transform.position = new Vector3(i * (usz + deta), j * (usz + deta), k * (usz + deta));
                        g.transform.position = 
                            IMAGECORE.gameObject.transform.right * (i * (usz + deta)) + 
                            Vector3.up * (j * (usz + deta))  + 
                            IMAGECORE.gameObject.transform.forward * (k * (usz + deta));
                        g.transform.position += IMAGECORE.gameObject.transform.position;

                        g.transform.rotation = IMAGECORE.gameObject.transform.rotation;
                        g.transform.SetParent(IMAGECORE.gameObject.transform);

                        itmlist.Add(g);
                        {// 文本
                            GameObject t = g.transform.GetChild(0).gameObject;
                            t.SetActive(true);
                            if (curnm != "")
                            {
                                TextMesh tm = t.GetComponent<TextMesh>();
                                tm.text = curnm;
                                tm.anchor = TextAnchor.MiddleLeft;
                                t.transform.position = g.transform.position + new Vector3(-(tm.text.Length * tm.characterSize * 0.075f) / 2, 0, -0.01f) / 4;
                            }
                        }
                    }
                }
            }
        }
    }
    void openbox()
    {
        int r = (int)(Mathf.Sqrt((float)minedat.Count)/2 + 2f);
        openBox(r, r);
    }
    // --------------------------------------------------
    // 旋转盒子  
    // --------------------------------------------------
    void rotboxX(float ang0, float actionalpha = 0f)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            IMAGECORE.gameObject.transform.localRotation = Quaternion.AngleAxis(ang0 + Mathf.LerpAngle(0, 90, actionalpha), Vector3.left);
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                rotboxX(ang0, actionalpha);
        }, 1 / 60.0f));

    }
    void rotboxY(float ang0, float actionalpha = 0f)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            IMAGECORE.gameObject.transform.localRotation = Quaternion.AngleAxis(ang0 + Mathf.LerpAngle(0, 90, actionalpha), Vector3.up);
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                rotboxY(ang0, actionalpha);  
        }, 1 / 60.0f));      
    }
    void rotboxZ(float ang0, float actionalpha = 0f)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            IMAGECORE.gameObject.transform.localRotation = Quaternion.AngleAxis(ang0 + Mathf.LerpAngle(0, 90, actionalpha), Vector3.forward);
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                rotboxZ(ang0, actionalpha);
        }, 1 / 60.0f));
    }
    // --------------------------------------------------
    // 缩放盒子  
    // --------------------------------------------------    
    void scalebox(float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            if (selectedbox == null)
                return;
            {
                float scl = Mathf.Lerp(0.25f, 0, actionalpha);
                for (int i = 0; i < itmlist.Count; i++)
                {
                    GameObject go = (GameObject)itmlist[i];
                    if (go != selectedbox)
                    {
                        go.transform.localScale = new Vector3(scl, scl, scl);
                    }
                }
            }
            {
               float scl = Mathf.Lerp(0.25f, 2.5f, actionalpha);
               selectedbox.transform.localScale = new Vector3(scl, scl, scl);
               selectedbox.transform.position = Vector3.Slerp(selectedbox.transform.position, IMAGECORE.gameObject.transform.position, 0.1f);
            }
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                scalebox(actionalpha);
            else
            {// 有没有子级
                //openbox();
            }
        }, 1 / 60.0f));
    }
    // --------------------------------------------------
    // 关闭盒子
    // --------------------------------------------------
    void closebox()
    {
        Debug.Log("PackCube.closebox");
        for (int i = 0; i < itmlist.Count; i++)
        {
            GameObject go = (GameObject)itmlist[i];
            GameObject.Destroy(go);
        }
        itmlist.Clear();
		selectedbox = null;
    }
    // -------------------------------------------------------------
    bool isClosed()
    {
        return (itmlist.Count == 0);
    }
    // --------------------------------------------------
    // 初始化
    // --------------------------------------------------
    void Start()
    {
        setupdat();

        RAWCUBE.SetActive(false);
        if (PMDLL.createMesh(this.gameObject, 1, 16))
        {// 造型
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
        }        
    }

    // --------------------------------------------------
    // GUI
    // --------------------------------------------------
    // 触发事件
    bool isselectready()
    {
        return selectedbox && selectedbox.transform.localScale.x > 1.5f;
    }
    void onItemEvent(GameObject go)
    {
        selectedbox = go;
        if(!isselectready())
            scalebox();
        else
        {// 这里操作原料块

        }
    }
    // --------------------------------------------------
    // 拖拽操作完成
    void onEndDragging()
    { 
        //GameObject dgo = Common.draginggo();
        //// 要扣除操作过的原料
        //if (dgo && dgo.name.Contains("RawCube"))
        //{
        //    string minename = dgo.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text;
        //    usemine(minename);            
        //}
        Debug.Log("关闭盒子");
        // 关闭盒子
        closebox();
        
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
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject == gameObject || hit.collider.gameObject.transform.parent == IMAGECORE.gameObject.transform))
                {
                    Common.settreated(true, gameObject);
                    if (hit.collider.gameObject.name.Contains("PackCube"))
                    {// 打开盒子
                        openbox();
                    }
                    else if (hit.collider.gameObject.name.Contains("RawCube"))
                    {// 操作
                        onItemEvent(hit.collider.gameObject);
                    }                 
                }
            }
            else if (Input.GetMouseButtonUp(0))
            {
                if(Common.isdragging() && Common.draginggo())
                {
                    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                    RaycastHit hit;
                    if (Common.mouseselect(ray, out hit))
                    { // 如果放在地上就是种植操作
                        Debug.Log("如果放在地上就是种植操作");
                        if (hit.collider.gameObject.name.Contains("MOON") || hit.collider.gameObject.name.Contains("Building"))
                        {
                            GameObject dgo = Common.draginggo();
                            if (dgo && dgo == selectedbox)
                            {                               
                                string minename = dgo.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text;
                                Debug.Log("minename" + minename);
                                GameObject bd = Instantiate(CUBEMUD);
                                bd.transform.GetChild(0).gameObject.name = minename;
                                bd.SetActive(true);
                                bd.transform.position = hit.point + hit.normal * 0.01f;
                                if (hit.collider.gameObject.name.Contains("MOON"))
                                    bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                                else
                                    bd.transform.rotation = hit.transform.rotation;

                                // 要扣除操作过的原料
                                usemine(minename);
                                closebox();
                            }
                        }
                    }
                }

                if (!Common.treated && !Common.isdragging())
                {
                    if (!isClosed())
                        Common.settreated(true, gameObject);
                    closebox();                   
                }      
            }            
        }
        if (e.type == EventType.MouseDrag && e.button == 0)
        {
            if (isselectready())
            {                                  
                // 设置    
                Common.dragbegin((() => { return selectedbox; }), onEndDragging);                 
                ((Collider)selectedbox.GetComponent<Collider>()).enabled = false;              
                             
            }
            {
                // 拖拽过程
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Common.mouseselect(ray, out hit) && selectedbox != null)
                {
                    selectedbox.transform.position = hit.point;
                }
            }
            Common.settreated(true, gameObject);
        }
    }
}
