using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class itemUI : MonoBehaviour
{
	public GameObject UI_ITEM;
	public GameObject PRODUCT;
	public GameObject BALL;
	public Texture[] textures;
	
	ArrayList itmlist = new ArrayList();
	Dictionary<string, ArrayList> itemmap = new Dictionary<string, ArrayList>();
	
	string selitmname = "root";
	ArrayList selclsnames = new ArrayList();
	
	float gtime = 0;
	float ld_mousetime = 0;
	Vector3 ld_mousepos; 
	RaycastHit lstPickHit;
	Vector3 cur_pos;
	string cur_state;
		
	static void setupItmDat(out Dictionary<string, ArrayList> itemmap)
	{
		itemmap = new Dictionary<string, ArrayList>();
		// 总分类
		itemmap["root"] = new ArrayList(new[] { "平台"});//, "岩石", "水", "土" "种植", "器具", "魔方" 
		// 一级 
		itemmap["种植"] = new ArrayList(new[] { "水草", "荷花", "松柏", "冬青", "花毯" }); // "棕榈树", "多肉", "芦苇", "盆栽", 
		itemmap["建筑"] = new ArrayList(new[] { "面", "屋顶", "门窗", "连接", "廊柱" });
		itemmap["器具"] = new ArrayList(new[] { "路灯", "栅栏"});//"喷泉", "合成器", "小车", "种子包","手电"，"灯笼", "轨道车",, "占星" 
		itemmap["色彩"] = new ArrayList(new[] { "红", "绿", "蓝", "黑", "白", "涂鸦", "雕塑" });//"黄", 
		itemmap["摧毁"] = new ArrayList(new[] { "删除", "恢复" });
		// 二级
		itemmap["面"]   = new ArrayList(new[] { "面块", "斜面", "块", "缩放" });
		itemmap["廊柱"] = new ArrayList(new[] { "方柱", "方拱门" });//,"圆柱","拱门"}); 
		itemmap["屋顶"] = new ArrayList(new[] { "尖顶", "圆顶", "平顶" });// "房檐"
		itemmap["门窗"] = new ArrayList(new[] { "随机窗", "方窗","日形窗", "田字窗", "圆形窗", "方形门", "拱形门"});
		itemmap["连接"] = new ArrayList(new[] { "园墙", "石阶", "台阶", "上阶" });       
	}
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
	bool isClosed()
	{// 特效跟数据都清理才算关闭
		return (selclsnames.Count <= 1 && itmlist.Count == 0);
	}

	void resetBox(string defaultname = "root")
	{
		if (selitmname == defaultname)
		{
			if(selclsnames.Count == 0)
				selclsnames.Add(defaultname);
			return;
		}
		closebox();
		selitmname = defaultname;
		selclsnames.Clear();
		selclsnames.Add(defaultname);
		itmlist.Clear();        
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
	}

	void SetMat(GameObject g, string nm)
	{
		Debug.Log("SetMat " + nm);
		//if (textureIndex.ContainsKey(nm))
		{
			Renderer rend = g.GetComponent<Renderer>();
			rend.material.mainTexture = textures[0];
		}
	}

	void openbox()
	{
		Debug.Log("openbox");
		
		selitmname = "建筑";
		
		selItem(selitmname);
		
		// clear
		closebox();

		// 各个选项特效
		ArrayList itms;
		itmGetChildren(getCurCls(), out itms);
		
		if (itms == null)
		{
			return;
		}
		for (int i = 0; i < itms.Count; i++)
		{
			GameObject g = Instantiate(UI_ITEM);
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
				t.transform.position = new Vector3(-(tm.text.Length * tm.characterSize * 0.05f), 0, -0.01f) / 4;
			}
		} 
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
    // Start is called before the first frame update
    void Start()
    {
        setupItmDat(out itemmap);  
		
		resetBox();
		
    }
	 // --------------------------------------
    void Update() {
		gtime += Time.deltaTime;
        if (itmlist.Count > 0)
        {
            float scaler = 1;
            float r = 1.05f + 0.1f * scaler;
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
        }
    }
	// void onItemEvent(GameObject go)
    // {
        // if (go.transform.childCount == 0 || go.transform.GetChild(0).gameObject.GetComponent<TextMesh>() == null)
        // {
            // return;
        // }     

        // string itmnm = go.transform.GetChild(0).gameObject.GetComponent<TextMesh>().text;
        // if (0 != selItem(itmnm))
        // {
            // openbox();
        // }
	// }
	static bool mouseselectex(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {       
        bool treated = false;
        int layerMask = 1 << 8; // 不包括
        if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        {           
            return true;
        }
        return false;
    }
	void OnGUI()
    {
		if (GUI.changed)
			return;
		
        Event e = Event.current;
		
		if (Input.GetMouseButtonDown(0))
		{
			closebox();
			
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			if(mouseselectex(ray, out lstPickHit))
			{
				
			}
			ld_mousetime = Time.time;
			ld_mousepos = Input.mousePosition;
			cur_state = "";
		}
		
        if (e.type == EventType.MouseDrag && e.button == 0)
		{
			Vector3 pp1 = Camera.main.WorldToScreenPoint(lstPickHit.point);
			Vector3 pp2 = Camera.main.WorldToScreenPoint(lstPickHit.point + lstPickHit.normal);
			Vector3 dir = (pp2 - pp1).normalized;
			Vector3 msdeta = (Input.mousePosition - ld_mousepos);
			float dot = Vector3.Dot(msdeta, dir);
			float dot0 = Vector3.Dot(msdeta.normalized, dir);
			Debug.Log("dot " + dot0.ToString());
			
			if (dot > 0 || dot0 > 0.88f)
			{
				cur_state = "drag";
				cur_pos = lstPickHit.point + lstPickHit.normal * 5;
			}
		}
		
		if (Input.GetMouseButtonUp(0))
		{
			if(cur_state == "drag" && lstPickHit.collider)
			{
				GameObject g = Instantiate(BALL);
				g.SetActive(true);
				g.transform.SetParent(lstPickHit.collider.gameObject.transform);
				g.transform.position = cur_pos;
			}
			else if (lstPickHit.collider && lstPickHit.collider.gameObject.CompareTag("edit"))
			{
				openbox();
			}
		}
	}
}