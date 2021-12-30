using UnityEngine;
using System.Collections;
// ===================================================
// 数据对象，充当代理
// ===================================================
public class PlantAgent
{
    const float cVisDis = 10;
    static GameObject INSTTEMPLATE = null;
    
    public GameObject instance = null;
    public string name;
    public Vector3 position;
    public float createtime = 0;
    public float growingtime = 0, lgtime = 0;
    
    public bool picked = false;
    public void Init(string _name)
    {
        return;
        if(INSTTEMPLATE == null)
        {
            INSTTEMPLATE = GameObject.Find("WildPlant");
            INSTTEMPLATE.SetActive(false);
        }
        // 名字(植物与产出名称相同)
        name = _name;        
    }
    public void Update()
    {
        // 生长, 无论可见与否
        growingtime += Time.deltaTime;
        
        {// 视觉效果更新
            if (!picked && (position - Camera.main.transform.position).magnitude < cVisDis)
            {
                if (!instance)
                {// 动态绑定
                    instance = GameObject.Instantiate(INSTTEMPLATE);
                    instance.SetActive(true);
                    instance.transform.position = position;
                    instance.transform.GetChild(0).name = name;
                    {// 文本
                        if ((position - Camera.main.transform.position).magnitude < cVisDis / 2)
                        {
                            GameObject t = instance.transform.GetChild(0).GetChild(0).gameObject;
                            t.SetActive(true);
                            TextMesh tm = t.GetComponent<TextMesh>();
                            tm.text = name;
                            tm.anchor = TextAnchor.MiddleLeft;
                            t.transform.position = instance.transform.GetChild(0).position + new Vector3(-(tm.text.Length * tm.characterSize * 0.075f) / 2, 0, -0.01f) / 4;
                        }
                    }
                }
                if (growingtime - lgtime >= 10)
                {// 每秒钟更新
                    instance.SendMessage("growing", growingtime);
                    lgtime = growingtime;
                  //  Debug.Log("SendMessage growing : " + growingtime.ToString());
                }
            }
            else if (instance)
            {
                Debug.Log("PlantAgent Destroy " + instance.ToString());
                GameObject.Destroy(instance);
                instance = null;               
            }
        }
    }
}
// ===================================================
// 植物管理器
// ===================================================
public class PlantMgr : MonoBehaviour {
    // ----------------------------------------------------
    // 容器与管理
    // ----------------------------------------------------
    ArrayList PlantAgents = new ArrayList();
    public void Start()
    {
        for (int i = 0; i < 100; i++)
        {
            PlantAgent flower = new PlantAgent();

            PlantAgents.Add(flower);
            flower.Init(ItemStaticData.sMineName[(int)Random.Range(0, ItemStaticData.sMineName.Length)]);
        }
    }
    public void Update()
    {
        for (int i = 0; i < PlantAgents.Count; i++)
        {
            ((PlantAgent)PlantAgents[i]).Update();
        }
    }
    void picked(GameObject go)
    {
        return;
        for (int i = 0; i < PlantAgents.Count; i++)
        {
            if(((PlantAgent)PlantAgents[i]).instance == go)
            {
                ((PlantAgent)PlantAgents[i]).picked = true;
                Debug.Log("picked " + go.ToString());
                break;
            }
        }
    }
}
