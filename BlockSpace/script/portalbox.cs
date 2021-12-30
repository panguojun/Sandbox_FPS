using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class portalbox : MonoBehaviour
{
	public GameObject editcam;
	public GameObject fpscam;
	
    // 模板
    public GameObject MOON;
    public GameObject ITEM;
    public GameObject PRODUCT;
	public GameObject BUILDING;
    public GameObject LINE;
    public GameObject ARACEAE;
    public GameObject BAMBOO;
    public GameObject HPLANT;

    public GameObject MAGICBOX;
    public GameObject PACKCUBE;
    public GameObject RAWCUBE;
    public GameObject SPOTLIGHT;
    public GameObject FIRELIGHT;
    public GameObject ROADLIGHT;    
    public GameObject CUBEMUD;
    public GameObject FEILIAO;
    public GameObject GRASSTMP;
    public GameObject ROCK;
    public GameObject LOTUS;
    public GameObject SONGBAI;
    public GameObject DONGQING;
    public GameObject PENQUAN;
    public GameObject TriangleArtPaper;
    public GameObject Triangle3DArt;
    public GameObject DASHU;
    public GameObject TRACK;
    public GameObject MAIGICCUBE;

    // 纹理
    public Texture[] textures;
	Dictionary<string, int> textureIndex = new Dictionary<string, int>();
    // UI特效
    ArrayList itmlist = new ArrayList();

    // 激光
   // ArrayList linelist = new ArrayList();
	// 生成物列表
	ArrayList pdlist = new ArrayList();
    // time
    float gtime = 0.0f;
    float gopenboxtime = 0.0f;
    // 各合成材料块的数量
    Dictionary<string, int> pddat = new Dictionary<string, int>();

    // 各项数据
    Dictionary<string, ArrayList> itemmap = new Dictionary<string, ArrayList>();
    string selitmname;
    public ArrayList selclsnames = new ArrayList();
    /// 一些操作逻辑
    Vector3 position0;
    Quaternion rotation0;
    GameObject lstpickeobj = null;
    string lstpickoper;

    float ld_mousetime = 0;
    Vector3 ld_mousepos;    
    float rd_mousetime = 0;
    Vector3 rd_mousepos;

    int operType = 0; // 0 其他操作 1 分割 2 拉伸
    public string currentOperName = "";

    // 挂点列表
    Vector3[] targets;
    // 建筑列表
    ArrayList buildinglist = new ArrayList();
    // 解密场景
    public bool byPuzzle = false;
    // --------------------------------------
    // 操作历史列表
    class oper_t
    {
        public string name;
        public GameObject obj;
        public int dat;
        public oper_t(string _name, GameObject _obj, int _dat)
        {
            name = _name;
            obj = _obj;
            dat = _dat;
        }
    };
    ArrayList operHistoryList = new ArrayList();
    void addOper(oper_t opr)
    {
        operHistoryList.Add(opr);
      //  Debug.Log("operHistoryList " + opr.name);
        if (operHistoryList.Count > 1)// 暂时设计为后退一步
        {            
            operHistoryList.RemoveRange(0, operHistoryList.Count - 1);
            Debug.Log("operHistoryList RemoveRange");
        }
    }
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
        return result;
    }
    string getCurCls()
    {
        return (string)selclsnames[selclsnames.Count - 1];
    }
    bool backCls()
    {
        bool result = false;
        Debug.Log("backCls" + selclsnames[0]);
        if (selclsnames.Count > 1)
        {
            Debug.Log("backCls RemoveAt" + selclsnames[1]);
            selclsnames.RemoveAt(selclsnames.Count - 1);
            selitmname = "延伸";            
            result = true;
        }
        return result;
    }
    bool isClosed()
    {// 特效跟数据都清理才算关闭
        return (selclsnames.Count <= 1 && itmlist.Count == 0);
    }
    void resetBox(string defaultname = "root")
    {
        if (selitmname == defaultname)
            return;
        closebox();
        selitmname = defaultname;
        selclsnames.Clear();
        selclsnames.Add(defaultname);
        itmlist.Clear();        
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
    // 取下合成后的产品
    void pickedupproduct(GameObject go)
	{
        Debug.Log("pickedupproduct " + go.ToString());
        string nm = go.name;
        if (!pddat.ContainsKey(nm))
            pddat[nm] = 1;
        else
            pddat[nm] += 1;
    }
    int getpdnum(string pdnm)
    {
        int itmnum = 0;        
        if (pddat.ContainsKey(pdnm))
        {
            itmnum = pddat[pdnm];
        }
       // Debug.Log("getpdnum " + pdnm + ":" + itmnum.ToString());
        return itmnum;
    }
	bool usepd(string pdnm)
	{
		int num = getpdnum(pdnm);
		if (num > 1)
		{
			pddat[pdnm] --;
			return true;
		}
		if(num == 1)
		{
			pddat.Remove(pdnm);
			return true;
		}
		return false;
	}
    void setupTextureIndices()
    {
        textureIndex["平台"] = 0;
        textureIndex["岩石"] = 1;
        textureIndex["魔方"] = 1;        
        textureIndex["水"] = 2;
        textureIndex["土"] = 3;
        textureIndex["水草"] = 4;
        textureIndex["多肉"] = 5;
        textureIndex["荷花"] = 6;
        textureIndex["棕榈树"] = 7;
        textureIndex["松柏"] = 8;
        textureIndex["冬青"] = 9;
        textureIndex["花毯"] = 10;
        textureIndex["面"] = 11;
        textureIndex["屋顶"] = 12;
        textureIndex["门窗"] = 13;
        textureIndex["连接"] = 14;
        textureIndex["廊柱"] = 15;
        textureIndex["路灯"] = 16;
        textureIndex["灯笼"] = 17;
        textureIndex["喷泉"] = 18;
        textureIndex["栅栏"] = 19;
        textureIndex["删除"] = 20;
        textureIndex["恢复"] = 21;
        textureIndex["面块"] = 22;
        textureIndex["斜面"] = 23;
        textureIndex["块"] = 24;
        textureIndex["缩放"] = 25;
        textureIndex["方柱"] = 26;
        textureIndex["方拱门"] = 27;
        textureIndex["尖顶"] = 28;
        textureIndex["平顶"] = 29;
        textureIndex["圆顶"] = 30;
        textureIndex["房檐"] = 31;
        textureIndex["随机窗"] = 32;
        textureIndex["方窗"] = 33;
        textureIndex["日形窗"] = 34;
        textureIndex["田字窗"] = 35;
        textureIndex["圆形窗"] = 36;
        textureIndex["方形门"] = 37;
        textureIndex["拱形门"] = 38;
        textureIndex["园墙"] = 39;
        textureIndex["石阶"] = 40;
        textureIndex["台阶"] = 41;
        textureIndex["上阶"] = 42;

        textureIndex["红"] = 43;
        textureIndex["黄"] = 44;
        textureIndex["绿"] = 45;
        textureIndex["蓝"] = 46;
        textureIndex["白"] = 47;
        textureIndex["黑"] = 48;
        textureIndex["涂鸦"] = 49;
        textureIndex["雕塑"] = 49;

    }
    void SetMat(GameObject g, string nm)
    {
        Debug.Log("SetMat " + nm);
        if (textureIndex.ContainsKey(nm))
        {
            Renderer rend = g.GetComponent<Renderer>();
            rend.material.mainTexture = textures[textureIndex[nm]];
        }
    }
	GameObject _Instantiate(GameObject g)
	{
		GameObject ng = Instantiate(g);
		ng.transform.parent = MOON.transform;
		return ng;
	}
    // --------------------------------------
    void openbox()
    {
        // 在地上不能打开
        if (this.transform.parent == null)
            return;
        Debug.Log("openbox");
        // TEST
        pddat["房间"] = 1;
        pddat["平台"] = 1;
        pddat["水"] = 1;
        pddat["土"] = 1;
        pddat["六角台"] = 1;     
        pddat["随机窗"] = 1;
        pddat["涂鸦"] = 1;
        pddat["雕塑"] = 1;
        pddat["日形窗"] = 1;
        pddat["圆形窗"] = 1;        
        pddat["尖顶"] = 1;
        pddat["平顶"] = 1;
        pddat["圆顶"] = 1;
        pddat["房檐"] = 1;
        pddat["园墙"] = 1;
        pddat["圆柱"] = 1;
        pddat["拱门"] = 1;
        pddat["方柱"] = 1;
        pddat["方拱门"] = 1;        
        pddat["随机面"] = 1;        
        pddat["上阶"] = 1;
        pddat["石阶"] = 1;
        pddat["台阶"] = 1;
        pddat["栅栏"] = 1;
        pddat["占星"] = 1;        
        pddat["小船"] = 1;
        pddat["轨道车"] = 1;
        pddat["延伸"] = 1;
        pddat["面块"] = 1;
        pddat["分割"] = 1;
        pddat["斜面"] = 1;
        pddat["缩放"] = 1;        
        pddat["块"] = 1;        
        pddat["方形门"] = 1;
        pddat["拱形门"] = 1;
        pddat["多肉"] = 1;
        pddat["水草"] = 1;
        pddat["冬青"] = 1;
        pddat["树"] = 1;
        pddat["喷泉"] = 1;        
        pddat["棕榈树"] = 1;
        pddat["松柏"] = 1;        
        pddat["小车"] = 1;
        pddat["土虫"] = 1;
        pddat["矿石"] = 1;
        pddat["合成器"] = 1;
        pddat["种子包"] = 1;
        pddat["手电"] = 1;
        pddat["灯笼"] = 1;
        pddat["路灯"] = 1;
        pddat["岩石"] = 1;
        pddat["魔方"] = 1;        
        pddat["假山"] = 1;
        pddat["荷花"] = 1;        
        pddat["花毯"] = 1;
        pddat["肥料"] = 1;
        pddat["红"] = 1;
        pddat["绿"] = 1;
        pddat["黄"] = 1;
        pddat["蓝"] = 1;
        pddat["白"] = 1;
        pddat["黑"] = 1;
        pddat["田字窗"] = 1;
        pddat["方窗"] = 1;

       // gopenboxtime = gtime;

        // clear
        closebox();

        // 各个选项特效
        ArrayList itms;
        itmGetChildren(getCurCls(), out itms);
        if (itms == null)
        {
            Common.debugerr("portalbox openbox itmGetChildren failed! " + getCurCls());
            return;
        }
        for (int i = 0; i < itms.Count; i++)
        {
            GameObject g = _Instantiate(ITEM);
            g.name = "itm" + (1 + i).ToString();
            g.SetActive(true);
            g.transform.SetParent(transform);
            itmlist.Add(g);
            SetMat(g, (string)itms[i]);
            {// 文本
                GameObject t = g.transform.GetChild(0).gameObject;
                t.SetActive(true);

                TextMesh tm = t.GetComponent<TextMesh>();
                tm.text = (string)itms[i];
                tm.anchor = TextAnchor.MiddleLeft;
                t.transform.position = new Vector3((tm.text.Length * tm.characterSize * 0.05f), 0, -0.01f) / 4;
            }
        }

        // 颜色
        Color tcor;
        if ((string)selclsnames[0] == "建筑")
        {
            tcor = new Color(0.25f, 0.5f, 1, 1);
        }
        else if((string)selclsnames[0] == "种植")
        {
            tcor = new Color(0, 1, 0, 1);
        }
        else if ((string)selclsnames[0] == "器具")
        {
            tcor = new Color(1, 0, 1, 1);
        }
        else if ((string)selclsnames[0] == "色彩")
        {
            tcor = Color.yellow;
        }
        else if ((string)selclsnames[0] == "摧毁")
        {
            tcor = Color.red;
        }
        else
        {
            tcor = Color.yellow;
        }
       // changeitemcolor(new Color(1, 1, 1, 1)); 
              	   
        PRODUCT.SetActive(true);
        Update();
        PRODUCT.SetActive(false);
        if (!itmHasChildren(selitmname) && selitmname != "延伸")
        {// 显示生成物的数量(只是一个效果）
            int itmnum = getpdnum(selitmname);
            if (itmnum > 0)
            {
                float us = 0.1f;
                float deta = 0.01f;
                for (int i = 1; i < itmnum; i++)
                {
                    GameObject go = _Instantiate(PRODUCT);
                    go.SetActive(true);
                    go.transform.localScale = new Vector3(us, us, us);
                    go.transform.position = PRODUCT.transform.position + PRODUCT.transform.forward * (i * (us + deta));
                    go.transform.rotation = PRODUCT.transform.rotation;
                    go.transform.SetParent(transform);
                    pdlist.Add(go);
                }
                //if (linelist.Count == 0)
                //{// 开启激光           
                //    GameObject line = (GameObject)_Instantiate(LINE);
                //    ((Line3d)line.GetComponent<Line3d>()).setLine((transform.position), PRODUCT.transform.position);
                //    line.SetActive(true);
                //    linelist.Add(line);
                //    line.transform.SetParent(transform);
                //}
                PRODUCT.SetActive(true);
            }            
        }
        PMDLL.setReal2(1);
        if (PMDLL.createMesh(this.gameObject, 1, 4))
        {// 造型
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
        }
    }   
    void clearpd()
    {
        //for (int i = 0; i < linelist.Count; i++)
        //{
        //    GameObject go = (GameObject)linelist[i];
        //    go.transform.SetParent(null);
        //    GameObject.Destroy(go);
        //}
        //linelist.Clear();
        for (int i = 0; i < pdlist.Count; i++)
        {
            GameObject go = (GameObject)pdlist[i];
            GameObject.Destroy(go);
        }
        pdlist.Clear();

        PRODUCT.SetActive(false);        
    }
    public void closebox()
    {
       // Debug.Log("closebox");
        for (int i = 0; i < itmlist.Count; i++)
        {
            GameObject go = (GameObject)itmlist[i];
            GameObject.Destroy(go);
        }
        itmlist.Clear();
        clearpd();
        PMDLL.setReal(g_ang0);
        PMDLL.setReal2(0);
        if (PMDLL.createMesh(this.gameObject, 1, 4))
        {// 造型
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
        }
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
    void changeitemcolor(Color cor)
    {
        {// 改变item颜色
            {
                Material mat = ((Renderer)ITEM.GetComponent<Renderer>()).material;
                mat.SetColor("_TintColor", cor);
            }
            { // 文字
                ITEM.transform.GetChild(0).gameObject.GetComponent<TextMesh>().color = Color.black;
            }
        }
    }
    // --------------------------------------
    void Start()
    {
        ITEM.SetActive(false);
        LINE.SetActive(false);
        PRODUCT.SetActive(false);
        //BUILDING.SetActive(false);
        ARACEAE.SetActive(false);
        BAMBOO.SetActive(false);
        HPLANT.SetActive(false);
        MAGICBOX.SetActive(false);
        PACKCUBE.SetActive(false);
        RAWCUBE.SetActive(false);
        FIRELIGHT.SetActive(false);
        SPOTLIGHT.SetActive(false);
        ROADLIGHT.SetActive(false);
        CUBEMUD.SetActive(false);
        FEILIAO.SetActive(false);
      //  TRIANGLEARTPAPER.SetActive(false);

        setupdat();
        resetBox();

        PMDLL.setReal(90);
        if (PMDLL.createMesh(this.gameObject, 1, 4))
        {// 造型
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
        }        

        // position0 = Camera.main.transform.InverseTransformPoint(transform.position);
        position0 = new Vector3(0, -0.294f, 0.611f);// Camera.main.transform.InverseTransformPoint(new Vector3(0, -0.294f, 0.611f));
        //rotation0 = transform.rotation * Quaternion.Inverse(Camera.main.transform.rotation);
        rotation0 = Quaternion.Euler(-28.711f, 0, 0);
		freemod.bfreewalk = false;
		
       // if(!LoadMesh.bnewcreated && !byPuzzle)
        {
            transform.parent = editcam.transform;
            transform.position = editcam.transform.TransformPoint(position0);
            transform.rotation = editcam.transform.rotation * rotation0;
            transform.localScale = new Vector3(0.06f, 0.06f, 0.06f);
            freemod.bfreewalk = true;
        }

        {// 挂点
            int cnt = PMDLL.gettargetcnt();
            if (cnt > 0)
            {
                targets = new Vector3[cnt];
                PMDLL.getTargets(targets);
            }
        }

        setupTextureIndices();
    }
    // --------------------------------------
    void Update() {
        gtime += Time.deltaTime;

        if (itmlist.Count > 0)
        {// 旋转UI特效
            float scaler = Mathf.Lerp(0, 1, (gtime - gopenboxtime) / 0.25f);
            float r = 0.05f + 0.1f * scaler;
            float r0 = 1 + 0.5f * scaler;
            float h = 0.2f + 0.1f * scaler;
            PRODUCT.transform.position = transform.position + transform.up * h;
            {// item
                float dang = 0.1F * gtime;
                for (int i = 0; i < itmlist.Count; i++)
                {
                    float ang = i * 2 * Mathf.PI / itmlist.Count + dang;
                    GameObject go = (GameObject)itmlist[i];

                    go.transform.position = PRODUCT.transform.position + transform.right * (r * Mathf.Cos(ang)) + transform.up * (r * Mathf.Sin(ang));
                    go.transform.localScale = new Vector3(-r0, r0, -1);
                    go.transform.LookAt(Camera.main.transform.position, transform.up);
                }
            }
            //if (PRODUCT.active)
            //{// 旋转产品演示特效
               // float dang = 0.1F;
               // PRODUCT.transform.Rotate(Vector3.up, dang);
            //}
            //if (linelist.Count > 0)
            //{// 激光颜色
            //    GameObject line = (GameObject)linelist[0];
            //    Material mat = ((Renderer)line.GetComponent<Renderer>()).material;
            //    mat.SetColor("_TintColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 0.1f), Random.Range(0.0f, 0.0f), Random.Range(0.0f, 0.5f)));
            //    line.SetActive(true);
            //    linelist.Add(line);
            //    line.transform.SetParent(transform);
            //}
        }
    }
    // --------------------------------------------------
    // GUI
    // --------------------------------------------------
    // 触发事件
    bool onItemEvent(GameObject go)
    {
        if (go.transform.childCount == 0 || go.transform.GetChild(0).gameObject.GetComponent<TextMesh>() == null)
        {
            Common.debugerr("onItemEvent failed!");
            return false;
        }     

        string itmnm = go.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text;
        if (0 != selItem(itmnm))
        {
            clearpd();
            openbox();
        }
        gopenboxtime = 0;
        setOperType(0);

        if (((string)selclsnames[0]) == "建筑")
        {
            PRODUCT.transform.localScale = Vector3.one * 0.001f;
        }
		else if(((string)selclsnames[0]) == "器具")
		{
			if(itmnm == "栅栏" || itmnm == "占星")
			{
				PRODUCT.transform.localScale = Vector3.one * 0.001f;
			}
			else
			{
				PMDLL.createMesh(PRODUCT, 4);
				PRODUCT.transform.localScale = Vector3.one * 10.0f;
			}
		}
		else if(((string)selclsnames[0]) == "root")
		{
			PMDLL.createMesh(PRODUCT, 4);
			PRODUCT.transform.localScale = Vector3.one * 10.0f;
		}
        //else if (((string)selclsnames[0]) == "色彩")
        //{
        //    PMDLL.createMesh(PRODUCT, 19);
        //    PRODUCT.transform.localScale = Vector3.one;
        //}
        else if (((string)selclsnames[0]) == "种植")
        {
            PMDLL.createMesh(PRODUCT, 17);
            // PMDLL.setUInt(new Color32(0, 255, 0, 255));
            if (itmnm == "荷花")
                PMDLL.setUInt(new Color32(255, 0, 255, 255));
            PRODUCT.transform.localScale = Vector3.one * 2;
            //PMDLL.setUInt(new Color32((byte)(PICK.s_brushcolor.r * 255), (byte)(PICK.s_brushcolor.g * 255), (byte)(PICK.s_brushcolor.b * 255), (byte)(PICK.s_brushcolor.a * 255)));
        }
        else
        {
            PRODUCT.transform.localScale = Vector3.one;
        }        
        {// 色彩
            if (itmnm == "红")
                PICK.s_brushcolor = Color.red;// Color.Lerp(PICK.s_brushcolor, Color.red, 0.25f);
            else if (itmnm == "绿")
                PICK.s_brushcolor = Color.green;// Color.Lerp(PICK.s_brushcolor, Color.green, 0.25f);
            else if (itmnm == "黄")
                PICK.s_brushcolor = Color.yellow;// Color.Lerp(PICK.s_brushcolor, Color.green, 0.25f);
            else if (itmnm == "蓝")
                PICK.s_brushcolor = Color.blue;// Color.Lerp(PICK.s_brushcolor, Color.blue, 0.25f);
            else if (itmnm == "白")
                PICK.s_brushcolor = Color.white;// Color.Lerp(PICK.s_brushcolor, Color.white, 0.25f);
            else if (itmnm == "黑")
                PICK.s_brushcolor = Color.black;// Color.Lerp(PICK.s_brushcolor, Color.black, 0.25f);
            
            if (selclsnames.Count >= 1 && (string)selclsnames[0] == "色彩")
            {// 颜色
             // Material mat = ((Renderer)PRODUCT.GetComponent<Renderer>()).material;
             // mat.SetColor("_TintColor", PICK.s_brushcolor);
                PMDLL.setUInt(new Color32((byte)(PICK.s_brushcolor.r * 255), (byte)(PICK.s_brushcolor.g * 255), (byte)(PICK.s_brushcolor.b * 255), (byte)(PICK.s_brushcolor.a * 255)));
                PMDLL.createMesh(PRODUCT, 19);
                PRODUCT.transform.localScale = new Vector3(1, 1, 1);
                //mat.EnableKeyword("_EMISSION");
                //mat.SetColor("_EmissionColor", PICK.s_brushcolor);

            }
            else
            {
                Material mat = ((Renderer)PRODUCT.GetComponent<Renderer>()).material;
                mat.SetColor("_TintColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 0.5f));
            }
        }

        {// 摧毁
            if (itmnm == "删除")
            {
                setcubestate(3);
                closebox();
                return true;
            }
            else if (itmnm == "恢复")
            {
                undo();
                closebox();
                return true;
            }
        }
        
        return false;
    }
    void onProductEvent(GameObject go)
    {
        Debug.Log("onProductEvent");
        closebox();

        PRODUCT.SetActive(true);
        setOperType(0);
    }
    // --------------------------------------------------
    public int Name2OperID(string operName)
    {        
        if     (operName == "延伸") return 0;
        else if(operName == "随机窗") return 1;
        else if (operName == "分割") return 3;
        else if (operName == "方形门") return 4;
        else if (operName == "凸出") return 5;
        else if (operName == "日形窗") return 6;
        else if (operName == "拱形门") return 7;
        else if (operName == "尖顶") return 8;
        else if (operName == "斜面") return 10;
        else if (operName == "面块") return 11;
        else if (operName == "栅栏") return 13;
        else if (operName == "上阶") return 16;
        else if (operName == "石阶") return 15;
        else if (operName == "台阶") return 14;
        else if (operName == "平顶") return 17;
        else if (operName == "圆顶") return 18;               
        else if (operName == "园墙") return 19;
        else if (operName == "圆柱") return 20;
        else if (operName == "拱门") return 21;
        else if (operName == "随机面") return 22;
        else if (operName == "分割面") return 23;
        else if (operName == "方柱") return 24;
        else if (operName == "方拱门") return 25;
        else if (operName == "块") return 26;
        else if (operName == "缩放") return 27;
        else if (operName == "岩石") return 28;
        else if (operName == "圆形窗") return 29;
        else if (operName == "水") return 30;
        else if (operName == "房檐") return 31;
        else if (operName == "土") return 32;
        else if (operName == "方窗") return 33;
        else if (operName == "田字窗") return 34;
        else if (operName == "花毯") return 35;
        return 0;
    }
    // --------------------------------------------------
    // 旋转荷花
    // --------------------------------------------------
    float g_ang0 = 90;
    float g_actionalpha = 1;
    void rotlotus(float ang1, float actionalpha = 0)
    {
        g_actionalpha = actionalpha;
        if (ang1 - g_ang0 > 180) ang1 -= 360;
        if (ang1 - g_ang0 < -180) ang1 += 360;

        StartCoroutine(DelayToInvoke.Invoke(() =>
        {            
            float angle = Mathf.Lerp(g_ang0, ang1, actionalpha);
            PMDLL.setReal(angle);
            if (PMDLL.createMesh(this.gameObject, 1, 4))
            {// 造型
                MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
            }
            actionalpha += Time.deltaTime * 2.0f;
            if (actionalpha < 1)
                rotlotus(ang1, actionalpha);
            else
            {
                PMDLL.setReal(ang1);
                if (PMDLL.createMesh(this.gameObject, 1, 4))
                {// 造型
                    MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                    mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;
                }
                Debug.Log("rotlotus " + ang1.ToString());
                g_ang0 = ang1;
                if (g_ang0 > 180) g_ang0 -= 360;
                if (g_ang0 < -180) g_ang0 += 360;
                g_actionalpha = 1;
                Common.clearlstselection();
            }
        }, 1 / 60.0f));
    }
    // --------------------------------------------------
    // 幻化
    // --------------------------------------------------    
    void turned(float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            float falpha = Mathf.Lerp(0, 1, actionalpha);
            PMDLL.setReal(falpha);
            PMDLL.createMesh(PRODUCT, 18, 5);            
            actionalpha += Time.deltaTime * 2.0f;
            if (actionalpha < 1)
                turned(actionalpha);
            else
            {
                PMDLL.setReal(0);
                PMDLL.createMesh(PRODUCT, 17);                
                Debug.Log("turned finished!");
            }
        }, 1 / 60.0f));
    }
    float g_actionalphaex = 0;
    void turnedex(System.Action action,  GameObject go, float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            float falpha = Mathf.Lerp(0, 1, actionalpha);
            PMDLL.setReal(falpha);         
            actionalpha += Time.deltaTime;
            g_actionalphaex = actionalpha;
            if (actionalpha > 0.1f)         
            {                
                action();
            }
            if (actionalpha < 1)
                turnedex(action, go, actionalpha);
            else
            {                
                PMDLL.setReal(1);
                action();
                Debug.Log("turnedex finished!");
            }
        }, 5));
    }
    // --------------------------------------------------
    // 用户输入操作
    // --------------------------------------------------
    GameObject lstOperGameObj;
    int cubestate = 0; // 0 准备中 1 预览 2 操作 3 删除

    RaycastHit lstPickHit;
    int lstHitTri = -1;
	//int lastTriangleB = -1;
    Ray lstray;
    bool isOperDecided = false;
    Vector3 gtxtanchorpos = Vector3.zero;
    string labeltxt;
    bool blstpickeditem = false;
    // --------------------------------------------------
    void setcubestate(int s)
    {
        cubestate = s;
       // Debug.Log("cubestate = " + cubestate.ToString());
    }
    void setOperType(int v)
    {
       // Debug.Log("operType = " + v.ToString());
        operType = v;
		if(operType != 0)
		{
			PRODUCT.transform.localScale = Vector3.one * 0.001f;
		}
		else
		{
			//PRODUCT.transform.localScale = Vector3.one;
		}
    }    
    void OnGUI()
    {
        Event e = Event.current;
        if (freemod.bfreewalk)
		{
			/// 信息显示
			if(currentOperName == "延伸")
			{// UI
				GUI.skin.label.normal.textColor = Color.gray;            
				GUI.Label(new Rect(gtxtanchorpos.x - 50, Screen.height - gtxtanchorpos.y, 150, 20), labeltxt);
			}
			else if (currentOperName == "分割")
			{
				GUI.skin.label.normal.textColor = Color.gray;
				GUI.Label(new Rect(gtxtanchorpos.x + 50, Screen.height - gtxtanchorpos.y, 150, 20), labeltxt);
			}		
			/// 回退
			if (e.type == EventType.KeyDown)
			{            
				//if(e.type == EventType.KeyDown && e.control)
				//    Debug.Log("e.type = " + e.type.ToString() + " e.control = " + e.control.ToString() + " e.keyCode = " + e.keyCode.ToString());
				/// 键盘
			   // if (e.control && 
				//    e.keyCode == (KeyCode.Z))
				if (e.control && (e.keyCode == KeyCode.Z || e.keyCode == KeyCode.X))
				{
					Debug.Log("undo");
					undo();
					return;
				}
			}
		}
            
        /// 鼠标点击 
        if (e.isMouse && Input.GetMouseButtonDown(0) && freemod.bfreewalk)
        {
            RaycastHit hit;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);            
                        
            if (cubestate == 1)
                setcubestate(2);

            blstpickeditem = false;
            bool bhit = Common.mouseselect(ray, out hit);
            if (bhit)
                lstpickeobj = hit.collider.gameObject;
            if (bhit && (hit.collider.gameObject == gameObject || hit.collider.gameObject.transform.parent == gameObject.transform))
            {
                Common.settreated(true, gameObject);
                if (hit.collider.gameObject.name.Contains("itm"))
                {
                    blstpickeditem = true;
                    if (onItemEvent(hit.collider.gameObject))
                        return;
                    if (PRODUCT.activeSelf)
                    {// 直接操作简化步骤
                        onProductEvent(hit.collider.gameObject);
                        if (cubestate == 0 || cubestate == 3)
                        {
                            setcubestate(1);
                            PRODUCT.SetActive(true);
                        }
                    }
                }
                else if (hit.collider.gameObject.name.Contains("PRODUCT"))
                {
                    onProductEvent(hit.collider.gameObject);
                    if (cubestate == 0 || cubestate == 3)
                    {
                        setcubestate(1);
                        PRODUCT.SetActive(true);
                    }
                }
            }
            else if (cubestate == 3 && freemod.bfreewalk)
            {// 摧毁 删除
                if (Common.mouseselect0(ray, out hit))
                {
                    if (((LoadMesh)MOON.GetComponent<LoadMesh>()).getObjByName(hit.collider.gameObject.name) != null)
                    {
                        hit.collider.gameObject.SetActive(false);
                       // Destroy(hit.collider.gameObject);
                        addOper(new oper_t("del", hit.collider.gameObject, 0));                        
                        Debug.Log("摧毁");
                        return;
                    }
                    
                    if (hit.collider.gameObject.transform.parent && 
                        ((LoadMesh)MOON.GetComponent<LoadMesh>()).getObjByName(hit.collider.gameObject.transform.parent.gameObject.name) != null)
                    {
                        hit.collider.gameObject.transform.parent.gameObject.SetActive(false);
                        // Destroy(hit.collider.gameObject);
                        addOper(new oper_t("del", hit.collider.gameObject.transform.parent.gameObject, 0));
                        Debug.Log("摧毁");
                        return;
                    }
                }
            }
            lstpickoper = "点选";
            if (cubestate != 2)
                currentOperName = selitmname;
            ld_mousepos = Input.mousePosition;
            ld_mousetime = Time.time;
            isOperDecided = false;
            lstHitTri = -1;
			//lastTriangleB = -1;			
            if (bhit && hit.collider.gameObject.name.Contains("Building"))
            {
                lstHitTri = hit.triangleIndex;
				//MeshCollider collider = hit.collider as MeshCollider;
				// lastTriangleB = PMDLL.getnexttri(
					// collider.sharedMesh.vertices, 
					// collider.sharedMesh.vertices.Length, 
					// collider.sharedMesh.triangles, 
					// collider.sharedMesh.triangles.Length, 
					// hit.triangleIndex * 3
					// ) / 3;				
                lstPickHit = hit;
                lstray = ray;                
            }
        }

        /// 鼠标拖拽
        
        if (e.type == EventType.MouseDrag && e.button == 0 && freemod.bfreewalk)
        {
            if (Vector3.Distance(Input.mousePosition, ld_mousepos) < 4 || Time.time - ld_mousetime < 0.2)
            {// 避免误操作
                Debug.Log("避免误操作~");
                return;
            }
            ((Collider)PRODUCT.GetComponent<Collider>()).enabled = false;
            // 清理预览
            if (lstOperGameObj)
                lstOperGameObj.GetComponent<PICK>().clearpreview();		
            
            if ((!PRODUCT.activeSelf || (string)selclsnames[0] == "色彩")
                && itmlist.Count == 0 && lstpickeobj == gameObject)
            {// 放下它 
				Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
				RaycastHit hit;
				bool bhit = Common.mouseselect(ray, out hit);
			
                if (Vector3.Distance(hit.point, transform.position) < 100 &&
                    (hit.collider.gameObject.name.Contains("MOON") ||
                    hit.collider.gameObject.name.Contains("ROCK") ||
                    hit.collider.gameObject.name.Contains("Building")))
                {
                    Common.settreated(true, gameObject);
                    transform.position = hit.point;// + Vector3.up * 0.1f;                    
                    if (hit.collider.gameObject.name.Contains("MOON"))
                        transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                    else
                        transform.rotation = hit.transform.rotation;
                    transform.localScale = new Vector3(0.25f, 0.25f, 0.25f);
                    transform.SetParent(null);
                    lstpickoper = "放下";
                }
            }
            else if(lstHitTri != -1) // 点击到建筑物
            {	
				Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
				RaycastHit hit;
				bool bhit = Common.mouseselect(ray, out hit);
				
				if ((!isOperDecided || currentOperName == "延伸")
                    && lstHitTri != -1 && lstPickHit.collider.gameObject.name.Contains("Building")					
                    )
                {// 判断手势 延伸
                    Vector3 pp1 = Camera.main.WorldToScreenPoint(lstPickHit.point);
                    Vector3 pp2 = Camera.main.WorldToScreenPoint(lstPickHit.point + lstPickHit.normal * 1);
                    Vector3 dir = (pp2 - pp1).normalized;
                    Vector3 msdeta = (Input.mousePosition - ld_mousepos);
                    float dot = Vector3.Dot(msdeta, dir);
                    //float msdetalen = msdeta.magnitude;
                    float dot0 = Vector3.Dot(msdeta.normalized, dir);
                    //dot = msdetalen / (dot + 0.0001f);
					Debug.Log("dot " + dot0.ToString());
                    if (isOperDecided && currentOperName == "延伸" && dot > 0 || dot0 > 0.88f)
                    {
                        //if (cubestate == 0 && operType == 0)
                        {
                            setOperType(2);
                            setcubestate(1);
                            // 延伸 预览
                            float raydis = lstPickHit.distance;
                            ((PICK)lstPickHit.collider.gameObject.GetComponent<PICK>()).hittest(lstray, 12, true, dot * Mathf.Lerp(0.001f, 0.05f, raydis / 50));
                            PRODUCT.transform.position = lstPickHit.point;

                            lstOperGameObj = lstPickHit.collider.gameObject;
                            currentOperName = "延伸";
                            isOperDecided = true;
                           // gtxtanchorpos = Input.mousePosition;
                            gtxtanchorpos = Camera.main.WorldToScreenPoint(lstPickHit.collider.gameObject.transform.TransformPoint(PMDLL.getVec()));
                            labeltxt = (PMDLL.getdraglen() / 10.0f).ToString() + " , " + (PMDLL.getdragwid() / 10.0f).ToString() + " , " + (PMDLL.getdraghgt() / 10.0f).ToString();
                            //Debug.Log("gtxtanchorpos = " + Camera.main.WorldToScreenPoint(lstPickHit.collider.gameObject.transform.TransformPoint(PMDLL.getVec())).ToString());                                                        
                        }
                    }
                }
				
                if (bhit && hit.collider.gameObject != null &&
                    hit.collider.gameObject.name.Contains("Building"))
                {
					//Debug.Log("分割墙壁xxx " + isOperDecided.ToString() + " " + lastTriangleB.ToString() + "," + lstHitTri.ToString() + "," + hit.triangleIndex.ToString() + currentOperName);
                    //(cubestate == 0 || !PRODUCT.activeSelf) && 
                    if ((!isOperDecided || currentOperName == "分割") && lstPickHit.collider != null && lstPickHit.collider.gameObject == hit.collider.gameObject                        
                        && lstPickHit.normal == hit.normal && lstHitTri != -1
						// && (
							// !isOperDecided && lstHitTri != -1 && (Mathf.Abs(lstHitTri - hit.triangleIndex) <= 1)
							// || isOperDecided && (hit.triangleIndex == lstHitTri || hit.triangleIndex == lastTriangleB) && currentOperName == "分割"
							// )
                        )
                    {// 分割墙壁                       
                        setOperType(1);
                        setcubestate(1);
                        if (!PRODUCT.activeSelf)
                            PRODUCT.SetActive(true);
                        if(PMDLL.getmanualside() == 0)
                        {
                            ray = lstray;
                        }
                        // selitmname = "分割";
                        PMDLL.setVec(hit.collider.transform.InverseTransformDirection((hit.point - lstPickHit.point).normalized));
                        ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 23, true);
                        lstOperGameObj = lstPickHit.collider.gameObject;
                        currentOperName = "分割";
                        isOperDecided = true;
                        PRODUCT.transform.position = hit.point;
                        gtxtanchorpos = Input.mousePosition;
                        labeltxt = (PMDLL.getclipA() / 10.0f).ToString() + " : " + (PMDLL.getclipB() / 10.0f).ToString();
                    }
                }
            }
        }    
        
        /// 鼠标移动  

        if (freemod.bfreewalk 
			&& operType == 0
            && cubestate == 1
            && e.button != 1
            && (Input.GetAxis("Mouse X") != 0 || Input.GetAxis("Mouse Y") != 0))
        {               
            if (
                (PRODUCT.activeSelf || selitmname != "")
                && itmlist.Count == 0
                && freemod.bfreewalk               
                )
            {
                ((Collider)PRODUCT.GetComponent<Collider>()).enabled = false;
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                bool bhit = Common.mouseselect(ray, out hit);
                if(bhit)
                {
                    PRODUCT.transform.position = hit.point;
                    //Debug.Log("hit.point = " + hit.point.ToString());
                }                
                if (bhit && hit.collider.gameObject.name.Contains("Building") && ((string)selclsnames[0]) != "色彩")
                {// 预览
                     if (operType == 0)
                     {
                         int type = Name2OperID(selitmname);                                                              
                         if (type == 0 && lstHitTri != -1)
                         {
                             if (!PRODUCT.activeSelf)
                                 type = 11;
                         }
                         if (type != 0)
                         {// 添加预览
                             currentOperName = selitmname;                            
                             ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, type, true);
                         }
                     }                    
                     if (lstOperGameObj && lstOperGameObj != hit.collider.gameObject)
                     {                       
                         lstOperGameObj.GetComponent<PICK>().clearpreview();
                     }
                    
                    lstOperGameObj = hit.collider.gameObject; 
                   
                    if (!PRODUCT.activeSelf)
                        PRODUCT.SetActive(true);
                }
                else
                {
                    if (lstOperGameObj)
                        lstOperGameObj.GetComponent<PICK>().clearpreview();

                    if (operType != 0 && PRODUCT.activeSelf)
                        PRODUCT.SetActive(false);                    
                }
            }
        }

        /// 右键点击

        if (e.isMouse && freemod.bfreewalk)
        {
            if (e.type == EventType.MouseDown && e.button == 1)
            {
                rd_mousepos = e.mousePosition;
                rd_mousetime = Time.time;
                if (cubestate == 1)
                    PRODUCT.SetActive(false);                
            }
            else if (e.type == EventType.MouseUp 
                && e.button == 1)
            {// 右键点击抬起
                if (Vector3.Distance(rd_mousepos, e.mousePosition) < 8 && (Time.time - rd_mousetime) < 2) // 必须是点击
                {
                    PMDLL.resetmanualside();
                    setcubestate(0);
                    PRODUCT.SetActive(false);

                    // 右键点击切换
                    if (operType != 0                     
                        )
                        setOperType(operType + 1);

                    if (lstOperGameObj)
                        lstOperGameObj.GetComponent<PICK>().clearpreview();
                    lstHitTri = -1;					
                }
                else
                {
                    if (cubestate == 1)
                    {
                       // if(lstOperGameObj && lstOperGameObj.GetComponent<PICK>().hasPreview())
                            PRODUCT.SetActive(true);                       
                    }
                }
				
				if(cubestate == 3)
				{//如果是删除状态要切换 避免误删
					setcubestate(0);
				}
            }
        }
        if (freemod.bfreewalk
			&& e.isMouse
            && (!PRODUCT.activeSelf || cubestate == 0)
            && (Input.GetMouseButtonUp(0) 
            || Input.GetMouseButtonUp(1) && Vector3.Distance(rd_mousepos, e.mousePosition) < 8 && (Time.time - rd_mousetime) < 2
            ))
        {
			PMDLL.resetmanualside();
            {
                Debug.Log("Common.treated = " + Common.treated.ToString());
                if (!Common.treated && itmlist.Count != 0 && !blstpickeditem)
                {
                    Common.settreated(true, gameObject);
                    if (backCls())
                    {
                        openbox();
                    }
                    else
                        closebox();
                    return;
                }
            }
        }
        if (freemod.bfreewalk
		   && e.isMouse
           && (Input.GetMouseButtonUp(0)
           || Input.GetMouseButtonUp(1))
           )
        {// UI锚点重置
            gtxtanchorpos.y = -1000;
        }

        if (!freemod.bfreewalk)
        {// 滚轮操作
            float wheel = Input.GetAxis("Mouse ScrollWheel");
            if (wheel != 0)
            {
				editcam.SetActive(true);
			    fpscam.SetActive(false);
				
                editcam.transform.parent = null;
                transform.parent = editcam.transform;
                transform.position = editcam.transform.TransformPoint(position0);
                transform.rotation = editcam.transform.rotation * rotation0;
                transform.localScale = new Vector3(0.06f, 0.06f, 0.06f);
                lstpickoper = "拾取";
				
				freemod.bfreewalk = true;
            }
        }
			// if (e.isMouse && Input.GetMouseButtonUp(1))
			// {
				// freemod.bfreewalk = false;
                // Camera.main.transform.position = transform.position + Vector3.up * 1;
				// editcam.SetActive(false);
			    // fpscam.SetActive(true);
				// fpscam.transform.position = transform.position + Vector3.up * 1;
			// }
        /// 左键点击抬起
		if (!freemod.bfreewalk && e.isMouse && Input.GetMouseButtonUp(0))
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            bool bhit = Common.mouseselect(ray, out hit);
			if (this.transform.parent != Camera.main.transform && bhit && hit.collider.gameObject == gameObject)
			{// 拾取
                editcam.gameObject.transform.parent = null;
                transform.parent = editcam.transform;
				transform.position = editcam.transform.TransformPoint(position0);
				transform.rotation = editcam.transform.rotation * rotation0;
				transform.localScale = new Vector3(0.06f, 0.06f, 0.06f);
				lstpickoper = "拾取";
				
				editcam.SetActive(true);
				fpscam.SetActive(false);
				freemod.bfreewalk = true;
				//Debug.Log(lstpickoper);
			}
		}		 
        else if (freemod.bfreewalk && e.isMouse && Input.GetMouseButtonUp(0))
        {
            //PMDLL.resetmanualside();
            // 延伸需要特别处理
            if (currentOperName == "延伸")
            {
                if (lstOperGameObj && lstOperGameObj.GetComponent<PICK>().hasPreview())// 必须有预览
                {
                    ((PICK)lstOperGameObj.GetComponent<PICK>()).lsthittest(0);
                    addOper(new oper_t("editface", lstOperGameObj, 0));
                    usepd(currentOperName);
                    setcubestate(0);
                    PRODUCT.SetActive(false);

                    if (lstOperGameObj)
                        lstOperGameObj.GetComponent<PICK>().clearpreview();
                    return;
                }
            }

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            bool bhit = Common.mouseselect(ray, out hit);
            if(bhit && hit.collider.gameObject == gameObject)
            {// 点中莲花则关闭操作
                PRODUCT.SetActive(false);
            }
            if (operType != 0 && (!bhit || hit.collider.gameObject != lstOperGameObj))
            {
                {// 取消操作
                    Debug.Log("取消操作 " + currentOperName);
                    setcubestate(0);
                    PRODUCT.SetActive(false);
                }
            }
            if (lstpickoper == "放下")
            {
                freemod.bfreewalk = false;
                Camera.main.transform.position = transform.position + Vector3.up * 1;
				editcam.SetActive(false);
			    fpscam.SetActive(true);
				fpscam.transform.position = transform.position + Vector3.up * 1;
            }
            else if (//itmlist.Count == 0 && 
                !PRODUCT.activeSelf && lstpickoper != "释放材料")
            {
                if (bhit && (hit.collider.gameObject == gameObject
                    || hit.collider.gameObject.name.Contains("Building")
                    || hit.collider.gameObject.name.Contains("MOON")
                    || hit.collider.gameObject.name.Contains("WATER")
                    || hit.collider.gameObject.name.Contains("MUD")
                    ))
                {
                   if (lstpickoper != "拾取"
                        && (Vector3.Distance(Input.mousePosition, ld_mousepos) < 8 && Time.time - ld_mousetime < 2) // 必须是点击
                        )
                    {// 打开盒子    
                        //closebox();                        
                        if (itmlist.Count == 0 && hit.collider.gameObject.name.Contains("Building"))
                        {// 切换到建筑包
                            if (itmlist.Count == 0)
                                gopenboxtime = gtime;
                            if (((string)selclsnames[0]) != "建筑")
                            {
                                resetBox("建筑");
                                rotlotus(0 + 90);
                            }
                            lstpickoper = "打开建筑盒子";
                            Debug.Log("打开盒子: 建筑 " + hit.collider.gameObject.name);
                        }
                        else if (itmlist.Count == 0
                            && (hit.collider.gameObject.name.Contains("WATER") || hit.collider.gameObject.name.Contains("MUD")))
                        {// 切换到植物
                            if (itmlist.Count == 0)
                                gopenboxtime = gtime;
                            if (((string)selclsnames[0]) != "种植")
                            {
                                resetBox("种植");
                                rotlotus(360 / 5 + 90);
                            }
                            lstpickoper = "打开种植盒子";
                            Debug.Log("打开盒子: 种植 " + hit.collider.gameObject.name);
                        }
                        else if (hit.collider.gameObject == gameObject)
                        {// 切换到材料包    
                            if(itmlist.Count == 0)     
                                gopenboxtime = gtime;
                            setcubestate(0);
                            if (hit.triangleIndex >= 9 && hit.triangleIndex < 35)
                            {
                                resetBox("器具");
                                rotlotus(360 / 5 * 3 + 90);
                            }
                            else
                            if (hit.triangleIndex >= 35 && hit.triangleIndex < 61)
                            {
                                resetBox("摧毁");
                               // setcubestate(3);
                                rotlotus(360 / 5 * 2 + 90);
                            }
                            else
                            if (hit.triangleIndex >= 61 && hit.triangleIndex < 87)
                            {
                                resetBox("种植"); 
                                rotlotus(360 / 5 + 90);
                            }
                            else
                            if (hit.triangleIndex >= 87 && hit.triangleIndex < 113)
                            {
                                resetBox("建筑");
                                rotlotus(0 + 90);
                            }
                            else
                            if (hit.triangleIndex >= 113 && hit.triangleIndex < 139)
                            {
                                resetBox("色彩");
                                rotlotus((360 / 5 * 4 + 90));
                            }
                            else
                            {
                                resetBox();
                            }

                            Debug.Log("triangleIndex = " + hit.triangleIndex.ToString());
                        }
                        if((hit.collider.gameObject == gameObject || itmlist.Count == 0) 
                            && !hit.collider.gameObject.name.Contains("MOON"))
                            openbox();
                    }
                }
            }
            else
            if ((cubestate == 2 || cubestate == 1 && operType != 0)
                && itmlist.Count == 0 && PRODUCT.activeSelf)
            {
                setcubestate(1);
                int operID = Name2OperID(currentOperName);
                if (bhit
                    && hit.collider.gameObject != gameObject && hit.collider.gameObject != PRODUCT)
                {
                   // turned();
                    lstpickoper = "释放材料";
                    if (hit.collider.gameObject.name.Contains("Building") && operID != 0)
                    {// 面上的造型艺术
                        if (lstOperGameObj && lstOperGameObj.GetComponent<PICK>().hasPreview())// 必须有预览
                        {
                            //((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, operID);
                            ((PICK)hit.collider.gameObject.GetComponent<PICK>()).lsthittest(operID);
                            addOper(new oper_t("editface", hit.collider.gameObject, operID));
                            usepd(currentOperName);

                            if (currentOperName == "分割")
                            {
                                setcubestate(0);
                                PRODUCT.SetActive(false);
                            }
                        }						
                    }
                    else if (hit.collider.gameObject.name.Contains("MOON")
                        || hit.collider.gameObject.name.Contains("WATER")
                        || hit.collider.gameObject.name.Contains("MUD")
                        || hit.collider.gameObject.name.Contains("TRACK")
                        )
                    {                       
                        if (currentOperName == "肥料")
                        {
                            if (hit.collider.gameObject && hit.collider.gameObject.name.Contains("CubeMud"))
                            {
                                GameObject bd = _Instantiate(FEILIAO);
                                bd.SetActive(true);
                                bd.transform.position = hit.transform.position + hit.transform.up * 0.05f;
                                //  bd.transform.rotation = hit.transform.rotation;
                                bd.transform.SetParent(hit.collider.gameObject.transform);
                                usepd(currentOperName);
                            }
                        }                       
                        else if (currentOperName == "芦苇")
                        {
                            GameObject bd = _Instantiate(BAMBOO);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "棕榈树")
                        {
                            GameObject bd = _Instantiate(HPLANT);
                            ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).type = -1;
                            Vector3 scale0 = Vector3.one * (0.25f * Random.Range(0.5f, 1));
                            bd.transform.localScale = scale0;
                            bd.SetActive(true);                            

                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);

                            PMDLL.createMesh(bd, 17, 0);
                            turnedex(
                            () =>
                            {
                                bd.transform.localScale = Vector3.Lerp(scale0, new Vector3(1, 1.5f, 1), g_actionalphaex);                                
                                PMDLL.araceae(bd, 5, Vector3.zero, Vector3.up * 2.5f, 0.001f,
                                 16,
                                 8,
                                 16
                                 );
                            }
                            , bd
                            );
                           // bd.transform.parent = hit.collider.gameObject.transform;
                        }                        
                        else if (currentOperName == "小车")
                        {
                            GameObject go = GameObject.Find("Rover");
                            if (go)
                            {
                                go.SetActive(true);
                                go.transform.position = hit.point + hit.normal * 2;
                            }
                        }
                        else if (currentOperName == "合成器")
                        {
                            GameObject bd = MAGICBOX;
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "种子包")
                        {
                            PACKCUBE.SetActive(true);
                            // RAWCUBE.SetActive(true);
                            PACKCUBE.transform.position = hit.point;
                            buildinglist.Add(PACKCUBE);
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "水草")
                        {
                            GameObject bd = _Instantiate(GRASSTMP);                            
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            //bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                           // bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (currentOperName == "荷花")
                        {
                            GameObject bd = _Instantiate(LOTUS);
                            ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).createtype = 1;
                            ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = -1;
                            Vector3 scale0 = Vector3.one * (0.25f * Random.Range(0.5f, 1));
                             bd.transform.localScale = scale0;
                            PMDLL.setReal(0);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);

                            PMDLL.createMesh(bd, 17, 0);
                            int dat = (int)Random.Range(0, 18); // 随机设置                            
                            turnedex(                            
                            () =>
                            {
                                bd.transform.localScale = Vector3.Lerp(scale0, Vector3.one, g_actionalphaex);
                                PMDLL.setInt(dat);
                                PMDLL.createMesh(bd, 13, 0);
                            }
                            ,bd
                            );
                           // bd.transform.parent = hit.collider.gameObject.transform;
                        }                       
                    }
                    
                    {// 跟环境无关的
                        if (currentOperName == "房间")
                        {
                            if (hit.collider.gameObject.name.Contains("MOON"))
                            {
                                GameObject bd = _Instantiate(BUILDING);
                                bd.name = "Building" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                bd.transform.rotation = GeoStaticData.getGoundRot(hit.point);//Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);
                            }
                        }
                        else if (currentOperName == "平台")
                        {
                            //if (hit.collider.gameObject.name.Contains("MOON"))
                            {
                                GameObject bd = _Instantiate(BUILDING);
                                bd.name = "Building" + bd.GetInstanceID().ToString();
                                ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = 5;
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                bd.transform.rotation = GeoStaticData.getGoundRot(hit.point);//Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);

                                //bd.transform.parent = hit.collider.gameObject.transform;
                            }
                        }
                        else if (currentOperName == "多肉")
                        {
                            if (!hit.collider.gameObject.name.Contains("MOON")
                                && !hit.collider.gameObject.name.Contains("WATER"))
                            {
                                GameObject bd = _Instantiate(ARACEAE);
                                bd.name = "Araceae" + bd.GetInstanceID().ToString();
                                ((Araceae)bd.GetComponent(typeof(Araceae))).type = -1;
                                Vector3 scale0 = Vector3.one * (0.25f * Random.Range(0.5f, 1));
                                bd.transform.localScale = scale0;
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);

                                PMDLL.createMesh(bd, 17, 0);
                                int dat = (int)Random.Range(0, 3);
                                turnedex(
                                () =>
                                {
                                    bd.transform.localScale = Vector3.Lerp(scale0, new Vector3(0.25f, 0.25f, 0.25f), g_actionalphaex);
                                    PMDLL.araceae(bd, dat, Vector3.zero, Vector3.up * 2.5f, 0.001f,
                                     16,
                                     8,
                                     16
                                     );
                                }
                                , bd
                                );
                               // bd.transform.parent = hit.collider.gameObject.transform;
                            }
                        }
                        else if (currentOperName == "手电")
                        {
                            SPOTLIGHT.SetActive(true);
                            buildinglist.Add(SPOTLIGHT);
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "灯笼")
                        {
                            GameObject bd = _Instantiate(FIRELIGHT);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "路灯")
                        {
                            GameObject bd = _Instantiate(ROADLIGHT);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                           // bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (currentOperName == "岩石")
                        {
                            GameObject bd = _Instantiate(ROCK);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                            //bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (currentOperName == "魔方")
                        {
                            GameObject bd = _Instantiate(MAIGICCUBE);
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(-hit.normal, Vector3.up);                            
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                            //bd.transform.parent = hit.collider.gameObject.transform;
                        }                        
                        else if (currentOperName == "假山")
                        {
                            GameObject bd = _Instantiate(ROCK);
                            ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = 12;
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                        }
                        else if (currentOperName == "盆栽2")
                        {
                            if (hit.collider.gameObject.name.Contains("Building"))
                            {
                                GameObject bd = _Instantiate(CUBEMUD);
                                bd.SetActive(true);
                                bd.transform.position = hit.point + hit.normal * 0.01f;
                                if (hit.collider.gameObject.name.Contains("MOON"))
                                    bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, hit.normal);
                                else
                                    bd.transform.rotation = hit.transform.rotation;

                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);
                                //bd.transform.GetChild(0).gameObject.name
                            }
                        }
                        else if (currentOperName == "紫藤")
                        {
                            if (//hit.collider.gameObject.name.Contains("MUD") && 
                                hit.normal.y < 0.25f)
                            {
                                GameObject bd = _Instantiate(HPLANT);
                                ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).type = 6;
                                ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).facing = hit.normal;

                                bd.name = "ZITENG" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                //bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);
                            }
                        }
                        else if (currentOperName == "松柏")
                        {
                            if (hit.collider.gameObject.name.Contains("MUD") || hit.collider.gameObject.name.Contains("Building"))
                            {
                                GameObject bd = _Instantiate(SONGBAI);
                                ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).type = -1;
                                Vector3 scale0 = Vector3.one * (0.25f * Random.Range(0.5f, 1));
                                bd.transform.localScale = scale0;
                                bd.name = "SONGBAI" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                // bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);

                                PMDLL.createMesh(bd, 17, 0);
                                turnedex(
                                () =>
                                {
                                    bd.transform.localScale = Vector3.Lerp(scale0, new Vector3(2, 3, 2), g_actionalphaex);
                                    PMDLL.araceae(bd, 6, Vector3.zero, Vector3.up * 2.5f, 0.001f,
                                     16,
                                     8,
                                     16
                                     );
                                }
                                , bd
                                );
                               // bd.transform.parent = hit.collider.gameObject.transform;
                            }
                        }
                        else if (currentOperName == "冬青")
                        {
                            if (hit.collider.gameObject.name.Contains("MUD") || hit.collider.gameObject.name.Contains("Building"))
                            {
                                GameObject bd = _Instantiate(DONGQING);
                                ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).type = -1;
                                Vector3 scale0 = Vector3.one * (0.25f * Random.Range(0.5f, 1));
                                bd.transform.localScale = scale0;
                                bd.name = "DONGQING" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                // bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);

                                PMDLL.createMesh(bd, 17, 0);
                                turnedex(
                                () =>
                                {
                                    bd.transform.localScale = Vector3.Lerp(scale0, new Vector3(1.5f, 2, 1.5f), g_actionalphaex);
                                    PMDLL.araceae(bd, 7, Vector3.zero, Vector3.up * 2.5f, 0.001f,
                                     16,
                                     8,
                                     16
                                     );
                                }
                                , bd
                                );
                               // bd.transform.parent = hit.collider.gameObject.transform;
                            }
                        }
                        else if (currentOperName == "树")
                        {
                            if (hit.collider.gameObject.name.Contains("MUD") || hit.collider.gameObject.name.Contains("Building"))
                            {
                                GameObject bd = _Instantiate(DASHU);
                                ((HorticulturalPlant)bd.GetComponent(typeof(HorticulturalPlant))).type = 8;

                                bd.name = "DASHU" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point;
                                // bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up) * Quaternion.AngleAxis(Random.Range(0, 180), Vector3.up);
                                addOper(new oper_t("add", bd, 0));
                                usepd(currentOperName);
                            }
                        }
                        else if (currentOperName == "喷泉")
                        {
                            GameObject bd = _Instantiate(PENQUAN);
                            bd.name = "PENQUAN" + bd.GetInstanceID().ToString();
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                           // bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (currentOperName == "轨道车")
                        {
                            GameObject bd = _Instantiate(TRACK);
                            bd.name = "TRACKROOT" + bd.GetInstanceID().ToString();
                            bd.SetActive(true);
                            bd.transform.position = hit.point;
                            bd.transform.rotation = Quaternion.LookRotation(Camera.main.transform.right, Vector3.up);
                            addOper(new oper_t("add", bd, 0));
                            usepd(currentOperName);
                          //  bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (currentOperName == "雕塑")
                        {
                            GameObject bd = _Instantiate(Triangle3DArt);
                            ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = 21;
                            bd.name = "Triangle3DArt" + bd.GetInstanceID().ToString();
                            bd.SetActive(true);
                            bd.transform.position = hit.point + hit.normal * 0.01f;

                            usepd(currentOperName);
                            addOper(new oper_t("add", bd, 0));
                            // bd.transform.parent = hit.collider.gameObject.transform;
                        }
                        else if (hit.collider.gameObject.name.Contains("Building"))
                        {
                            if (currentOperName == "红")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "黄")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "绿")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "蓝")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "白")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "黑")
                            {
                                ((PICK)hit.collider.gameObject.GetComponent<PICK>()).hittest(ray, 1001);
                                usepd(currentOperName);
                                addOper(new oper_t("editface", hit.collider.gameObject, 0));
                            }
                            else if (currentOperName == "涂鸦")
                            {
                                GameObject bd = _Instantiate(TriangleArtPaper);
                                ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = 10;
                                bd.name = "TriangleArt" + bd.GetInstanceID().ToString();
                                bd.SetActive(true);
                                bd.transform.position = hit.point + hit.normal * 0.01f;
                                bd.transform.rotation = Quaternion.LookRotation(-hit.normal, Camera.main.transform.right);
                                usepd(currentOperName);
                                addOper(new oper_t("add", bd, 0));
                              //  bd.transform.parent = hit.collider.gameObject.transform;
                            }                           
                        }
                        //else if (hit.collider.gameObject.name.Contains("MOON"))
                        //{
                        //    if (currentOperName == "红")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);                               
                        //    }
                        //    else if (currentOperName == "黄")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);
                        //    }
                        //    else if (currentOperName == "绿")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);
                        //    }
                        //    else if (currentOperName == "蓝")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);
                        //    }
                        //    else if (currentOperName == "白")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);
                        //    }
                        //    else if (currentOperName == "黑")
                        //    {
                        //        Color cor0 = RenderSettings.skybox.GetColor("_SkyTint");
                        //        RenderSettings.skybox.SetColor("_SkyTint", Color.Lerp(cor0, PICK.s_brushcolor, 0.1f));
                        //        usepd(currentOperName);
                        //    }                           
                        //}
                    }
                }
                // PRODUCT.SetActive(false);
            }
            ((Collider)PRODUCT.GetComponent<Collider>()).enabled = true;
            if (lstOperGameObj)
                lstOperGameObj.GetComponent<PICK>().clearpreview();
			PMDLL.resetmanualside();
        }        
    }
    public void addtrioper(GameObject go)
    {
        addOper(new oper_t("tri", go, 0));
    }
    public void addtri3doper(GameObject go)
    {
        addOper(new oper_t("tri3D", go, 0));
    }
    public void undo()
    {// 撤销操作
        if (operHistoryList.Count > 0)
        {
            oper_t opr = (oper_t)operHistoryList[operHistoryList.Count - 1];
            if (opr.name == "add")
            {
                Debug.Log("undo add" + opr.name);
                DestroyObject(opr.obj);
                operHistoryList.RemoveAt(operHistoryList.Count - 1);
            }
            else if(opr.name == "editface")
            {
                Debug.Log("undo editface" + opr.name);
                ((PICK)opr.obj.GetComponent<PICK>()).undo(opr.dat);
                operHistoryList.RemoveAt(operHistoryList.Count - 1);
            }
            else if(opr.name == "del")
            {
                Debug.Log("undo del" + opr.name);
                opr.obj.SetActive(true);
                operHistoryList.RemoveAt(operHistoryList.Count - 1);
            }
            else if (opr.name == "tri")
            {
                Debug.Log("undo tri" + opr.name);
                ((TriangleArt)opr.obj.GetComponent<TriangleArt>()).undo();
                operHistoryList.RemoveAt(operHistoryList.Count - 1);
            }
            else if (opr.name == "tri3D")
            {
                Debug.Log("undo tri3D" + opr.name);
                ((TriangleArt3D)opr.obj.GetComponent<TriangleArt3D>()).undo();
                operHistoryList.RemoveAt(operHistoryList.Count - 1);
            }
        }
    }
}