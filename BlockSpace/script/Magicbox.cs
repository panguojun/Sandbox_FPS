using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class Magicbox : MonoBehaviour {
    // 模板
    public GameObject ITEM;
    public GameObject PRODUCT;	
	public GameObject LINE;
    public GameObject RAWCUBE;
    public GameObject PACKCUBE;
    public GameObject PORTALBOX;    

    // 模型上的坐标点
    Vector3[] targets1;
    // UI特效
    ArrayList itmlist = new ArrayList();
    // 合成激光
    ArrayList hecheng_line = new ArrayList();
    // 原料
    ArrayList rawlist = new ArrayList();
    // 当前合成公式的原料列表
    int currawlist = -1;
    // time
    float gtime = 0.0f;
    // 旋转控制
    bool brotrawcubes = true;

    // 各项数据
    Dictionary<string, ArrayList> itemmap;
    string selitmname = "root";
    ArrayList selclsnames = new ArrayList();

    // 自动匹配的矿物 合成后一起删除
    ArrayList automineused = new ArrayList();
    // --------------------------------------
    int selItem(string itmnm)
    {
        int result = 0; // 0 没有改变任何东西， 1 改变了 selitmname， 2 改变了 selclsnames, 3 都改变了
        if (selitmname != itmnm)
        {
            selitmname = itmnm;
            result |= 0x01;
        }
        if (itmHasChildren(itmnm))
        {
            if ((string)selclsnames[selclsnames.Count - 1] != itmnm)
            {
                selclsnames.Add(itmnm);
                result |= 0x02;
            }
        }
        else
        {// 当前合成公式的原料列表
            for (int i = 0; i < ItemStaticData.rawmap.GetLength(0); i++)
            {
                if (itmnm == ItemStaticData.rawmap[i, 0])
                    currawlist = i;
             }
        }
        return result;
    }
    string getCurCls()
    {
        return (string)selclsnames[selclsnames.Count - 1];
    }
    bool backCls()
    {
        bool result = false;
        Debug.Log("backCls");
        if (selclsnames.Count > 1)
        {
            selclsnames.RemoveAt(selclsnames.Count - 1);
            selitmname = "";
            result = true;
        }
        return result;
    }
    bool isClosed()
    {// 特效跟数据都清理才算关闭
        return (selclsnames.Count <= 1 && itmlist.Count == 0);
    }
    // --------------------------------------
    void setupdat()
    {
        ItemStaticData.setupItmDat(out itemmap);
    }
    bool itmHasChildren(string itmnm)
    {
        return itemmap.ContainsKey(itmnm);
    }
    bool itmGetChildren(string itmnm, out ArrayList itms)
    {
        if (!itmHasChildren(itmnm))
        {
            itms = null;
            return false;
        }

        itms = itemmap[itmnm];
        return true;
    }
    // --------------------------------------
    bool isopen()
    {
        return !(rawlist.Count == 0 && itmlist.Count == 0);
    }
    // --------------------------------------
    void openbox()
    {
        // clear
        closebox();

        if(currawlist != -1)
        {// 原料特效
            for (int i = 1; i < ItemStaticData.rawmap.GetLength(1); i++)
            {
                if (ItemStaticData.rawmap[currawlist, i] != "")
                {
                    GameObject g = Instantiate(RAWCUBE);
                    g.SetActive(true);
                    {// 颜色
                        Material mat = ((Renderer)(g).GetComponent<Renderer>()).material;                        
                        if(PackCube.minedat.ContainsKey(ItemStaticData.rawmap[currawlist, i]))
                        {// 自动检查矿物是否足够
                            mat.SetColor("_Color", new Color(1, 0, 0, 0.5f));
                        }
                    }
                    g.transform.SetParent(transform);
                    rawlist.Add(g);
                    {// 文本
                        GameObject t = g.transform.GetChild(0).gameObject;
                        t.SetActive(true);

                        TextMesh tm = t.GetComponent<TextMesh>();
                        tm.text = ItemStaticData.rawmap[currawlist, i];
                        tm.anchor = TextAnchor.MiddleLeft;
                        t.transform.position = g.transform.position + new Vector3((tm.text.Length * tm.characterSize * 0.05f), 0, -0.01f) / 4;
                        t.transform.localScale = new Vector3(-0.1F, 0.1F, 0.1F);
                    }
                }
            }
            PRODUCT.name = ItemStaticData.rawmap[currawlist, 0];
            PRODUCT.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text = PRODUCT.name;
        }
        else
        {// 选项UI
            ArrayList itms;
            itmGetChildren(getCurCls(), out itms);
            if (itms == null)
            {
                Common.debugerr("Magicbox openbox itmGetChildren failed! " + getCurCls());
                return;
            }

            // 颜色
            changeitemcolor(new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 0.5f), 0.5f));

            // 创建选项UI
            for (int i = 0; i < itms.Count; i++)
            {
                GameObject g = Instantiate(ITEM);
                g.name = "itm" + (1 + i).ToString();
                g.SetActive(true);
                g.transform.SetParent(transform);
                itmlist.Add(g);
                {// 文本
                    GameObject t = g.transform.GetChild(0).gameObject;
                    t.SetActive(true);

                    TextMesh tm = t.GetComponent<TextMesh>();
                    tm.text = (string)itms[i];
                    tm.anchor = TextAnchor.MiddleLeft;
                    t.transform.position = new Vector3((tm.text.Length * tm.characterSize * 0.075f), 0, -0.01f) / 4;
                }
            }
        }
        Update();
        PRODUCT.transform.localScale = new Vector3(0.25f, 0.25f, 0.25f);
        PRODUCT.transform.position = transform.position + new Vector3(0, 0.75f, 0);
        PRODUCT.SetActive(true);        
    } 
    //  清理激光   
    void clearlaser()
    {
        for (int i = 0; i < hecheng_line.Count; i++)
        {
            GameObject go = (GameObject)hecheng_line[i];
            GameObject.Destroy(go);
        }
        hecheng_line.Clear();
    }
    void closebox()
    {
        for (int i = 0; i < itmlist.Count; i++)
        {
            GameObject go = (GameObject)itmlist[i];
            GameObject.Destroy(go);
        }
        itmlist.Clear();
        clearlaser();             
        for (int i = 0; i < rawlist.Count; i++)
        {
            GameObject go = (GameObject)rawlist[i];
            GameObject.Destroy(go);
        }
        rawlist.Clear();        
        PRODUCT.SetActive(false);
    }
    // --------------------------------------
    void changedemocolor()
    {
        {// 改变生成物的颜色
            Material mat = ((Renderer)PRODUCT.GetComponent<Renderer>()).material;
            mat.SetColor("_TintColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 0.5f));           
        }
    }
    // --------------------------------------
    // 激光
    // --------------------------------------
    void hecheng_laser(GameObject go)
    {
        if (hecheng_line.Count == 0)
        {// 开启激光
            for (int i = 0; i < rawlist.Count; i++)
            {
                GameObject line = (GameObject)Instantiate(LINE);
                ((Line3d)line.GetComponent<Line3d>()).setLine(((GameObject)rawlist[i]).transform.position, PRODUCT.transform.position);
                line.SetActive(true);
                hecheng_line.Add(line);
            }
        }
    }

    // --------------------------------------
    void changeitemcolor(Color cor)
    {
        {// 改变item颜色
            Material mat = ((Renderer)ITEM.GetComponent<Renderer>()).material;
            mat.SetColor("_TintColor", cor);
        }
    }
    // --------------------------------------
    void Start()
    {
        ITEM.SetActive(false);
        LINE.SetActive(false);
        PRODUCT.SetActive(false);

        selclsnames.Add("root");
        setupdat();
		
		PMDLL.setup();
		
        if (PMDLL.createMesh(this.gameObject, 1, 8))
        {// 造型
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
            // 挂点
            int cnt = PMDLL.gettargetcnt();
            targets1 = new Vector3[cnt];
            PMDLL.getTargets(targets1);
        }
    }
    // --------------------------------------------------
    void Update()
    {
        gtime += Time.deltaTime;
        {
            float dang = 0.1F * gtime;
            float r = 1.0f;
            for (int i = 0; i < itmlist.Count; i++)
            {
                float ang = i * 2 * Mathf.PI / itmlist.Count + dang;
                GameObject go = (GameObject)itmlist[i];
                go.transform.position = transform.position + transform.TransformVector(new Vector3(r * Mathf.Cos(ang), 0.5f, r * Mathf.Sin(ang)));
                go.transform.localScale = new Vector3(0.5f, 0.5f, -1);
                go.transform.LookAt(Camera.main.transform.position, transform.up);
            }
        }
        if (brotrawcubes && hecheng_line.Count == 0 || currawlist != -1 && PRODUCT.activeSelf == false)
        {
            float dang = 0.1F * gtime;
            float r = 1.5f;
            for (int i = 0; i < rawlist.Count; i++)
            {
                float ang = i * 2 * Mathf.PI / rawlist.Count + dang;
                GameObject go = (GameObject)rawlist[i];
                go.transform.position = transform.position + transform.TransformVector(new Vector3(r * Mathf.Cos(ang), 0.75f, r * Mathf.Sin(ang)));
                go.transform.localScale = new Vector3(-0.5f, 0.5f, 0.5f);               
            }
        }
        if(currawlist == -1)
        {
            PRODUCT.SetActive(false);
        }
    }
    // --------------------------------------------------
    // GUI
    // --------------------------------------------------
    // 触发事件
    void onItemEvent(GameObject go)
    {
        if (go.transform.childCount == 0 || go.transform.GetChild(0).gameObject.GetComponent<TextMesh>() == null)
        {
            Common.debugerr("onItemEvent failed!");
            return;
        }
        if (0 != selItem(go.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text))
        {    
            openbox();
            changedemocolor();
        }
       
        Debug.Log("TriggerEvent: " + go.name);
    }
    // --------------------------------------------------
    void generating(float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {           
            {// 生成物
                float scl = Mathf.Lerp(0.25f, 0.5f, actionalpha);
                PRODUCT.transform.localScale = new Vector3(scl, scl, scl);
                PRODUCT.transform.Translate(Vector3.up * 0.01f);
            }
            {// 原料
                float scl = Mathf.Lerp(0.25f, 0, actionalpha);
                for (int i = 0; i < rawlist.Count; i++)
                {
                    GameObject go = (GameObject)rawlist[i];
                    go.transform.localScale = new Vector3(scl, scl, scl);
                }
            }
            {// 激光
                for (int i = 0; i < hecheng_line.Count; i++)
                {
                    GameObject go = (GameObject)hecheng_line[i];
                    // 激光颜色
                    Material mat = ((Renderer)go.GetComponent<Renderer>()).material;
                    mat.SetColor("_TintColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 0.1f), Random.Range(0.0f, 0.0f), Mathf.Lerp(1f, 0, actionalpha)));
                }
            }
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                generating(actionalpha);
            else
            {
                closebox();               
                PRODUCT.active = true;
            }
        }, 1 / 60.0f));       
    }
    // --------------------------------------------------
    bool checkproducing()
    {
        bool result = true;
        for(int i = 0; i < rawlist.Count; i ++)
        {
            Material mat = ((Renderer)((GameObject)rawlist[i]).GetComponent<Renderer>()).material;
          //  Debug.Log(mat.GetColor("_Color").ToString());          
            if(mat.GetColor("_Color").g < 0.5f)
            {
                result = false;
                break;
            }
        }
        return result;
    }
    bool isproductready()
    {
        return PRODUCT.transform.localScale.x > 0.45;
    }
    void onProductEvent(GameObject go)
    {
        if (currawlist == -1)
            return;        
        if (!isproductready())
        {// 开始合成
            if (checkproducing())
            {                
                generating();
                hecheng_laser(go);
            }
        }
        else
        {// 这里是取下逻辑    
            PORTALBOX.SendMessage("pickedupproduct", go);
            closebox();

            {// 这里扣除原料 
                for (int i = 0; i < rawlist.Count; i++)
                {
                    GameObject g = (GameObject)rawlist[i];
                    GameObject t = g.transform.GetChild(0).gameObject;
                    TextMesh tm = t.GetComponent<TextMesh>();
                    PackCube.usemine(tm.text);
                    Debug.Log("合成这里扣除原料: " + tm.text);
                }
            }

        }
    }
    // --------------------------------------------------
    void sittingrawcube(GameObject go, GameObject rawgo, Vector3 spos, Vector3 tpos, float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            if (!go || !rawgo)
            {
                Debug.Log("ERROR: sittingrawcube (!go || !rawgo)");
                return;
            }
            {
                Vector3 pos = Vector3.Lerp(spos, tpos, actionalpha);
                go.transform.position = pos;
                go.transform.rotation = Quaternion.Slerp(go.transform.rotation, rawgo.transform.rotation, 0.1f);
            }

            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                sittingrawcube(go, rawgo, spos, tpos, actionalpha);
            else
            {
                brotrawcubes = true;
                Common.dragend();

                {// 改变颜色
                    Material mat = ((Renderer)rawgo.GetComponent<Renderer>()).material;
                    mat.SetColor("_Color", new Color(0, 1, 0));
                }
            }
        }, 1 / 60.0f));
    }
    // --------------------------------------------------
    void onDragOnRawCube(GameObject go)
    {
        GameObject draginggo = Common.draginggo();
        if (draginggo == null)
            return;
        {// 检查数值
            string tnm = go.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text; 
            string rnm = draginggo.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text;
            if(tnm != rnm)
            {
                Common.screenprint("矿物不匹配！");
                return;
            }
        }

        // 动画特效
        brotrawcubes = false;
        sittingrawcube(draginggo, go, draginggo.transform.position, go.transform.position);
        Debug.Log("sittingrawcube " + draginggo.ToString() + "->" + go.ToString());
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
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject == gameObject || hit.collider.gameObject.transform.parent == gameObject.transform))
                {
                    Common.settreated(true, gameObject);
                    if (hit.collider.gameObject.name.Contains("Magicbox"))
                    {// 打开盒子
                        if(!isopen())
                            openbox();                       
                    }
                    else if (hit.collider.gameObject.name.Contains("itm"))
                    {// 操作
                        onItemEvent(hit.collider.gameObject);
                    }
                    else if (hit.collider.gameObject == PRODUCT)
                    {// 操作
                        onProductEvent(hit.collider.gameObject);
                    }
                }                
            }
            else if(Input.GetMouseButtonUp(0))
            {              
                if (!Common.treated && !Common.isdragging())
                {
                    if (!isClosed())
                        Common.settreated(true, gameObject);

                    if (currawlist != -1 && hecheng_line.Count != 0)
                    {
                        openbox();
                    }
                    else if (currawlist != -1)
                    {
                        currawlist = -1;
                        openbox();
                    }
                    else
                    {// 返回或者关闭
                        if (backCls())
                            openbox();
                        else
                            closebox();
                    }
                }
                else if(Common.isdragging())
                {// 原料被拖拽到目标上 
                    bool operfailed = true;
                    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                    RaycastHit hit;
                    if (Common.mouseselect(ray, out hit))
                    {                        
                        for (int i = 0; i < rawlist.Count; i ++)
                        {
                            GameObject go = (GameObject)rawlist[i];
                            if(hit.collider && go == hit.collider.gameObject)
                            {
                                operfailed = false;
                                onDragOnRawCube(go);
                                break;
                            }
                        }
                    }

                    if(operfailed)
                    {
                        // 半途取消后清理状态
                        Common.dragcanceled();
                    }
                }
            }
        }        

    }

}
