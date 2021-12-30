using UnityEngine;
using System.Collections;
// ----------------------------------------------------
class Fish
{
    const float cVisDis = 100;
    public static GameObject FISH;
    GameObject instance = null;
    int state; // 0 默认状态 
    Vector3 position;
    Vector3 direction;
    Vector3[] border;
    float speed;
    float lststatechangetime = 0;
    float statechangetime = 2;
    public void init()
    {        
        state = 0;
        speed = Random.Range(0.01f, 0.5F);
        direction = Quaternion.AngleAxis(Random.Range(0, 360), Vector3.up) * Vector3.forward;
    }
    public void Update()
    {
        {// 显示/隐藏
            if ((position - Camera.main.transform.position).magnitude < cVisDis)
            {
                if (!instance)
                {// 动态绑定
                    instance = GameObject.Instantiate(FISH);
                }
                instance.SetActive(true);
                instance.transform.position = position;
                instance.transform.rotation = Quaternion.LookRotation(direction);
            }
            else if (instance)
            {
                instance.SetActive(false);
            }
        }

        {// AI
            position += direction * speed * Time.deltaTime;

            if ((Time.time - lststatechangetime) > statechangetime)
            {
                lststatechangetime = Time.time;
                statechangetime = Random.Range(1, 8);
                speed = Random.Range(0.01f, 0.5F);
                direction = Quaternion.AngleAxis(Random.Range(0, 360), Vector3.up) * Vector3.forward;
            }
        }
    }
}
// ----------------------------------------------------
public class FishMgr : MonoBehaviour
{   
    public GameObject FISH;
    ArrayList fishlist = new ArrayList();
    // Use this for initialization
    void Start ()
    {
        Fish.FISH = FISH;
        for (int i = 0; i < 18; i++)
        {
            Fish fish = new Fish();
            fishlist.Add(fish);
            fish.init();
        }
    }	
	// Update is called once per frame
	void Update ()
    {
        for (int i = 0; i < fishlist.Count; i++)
        {
            ((Fish)fishlist[i]).Update();
        }
    }
}
