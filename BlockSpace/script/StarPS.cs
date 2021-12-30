using UnityEngine;
using System.Collections;

public class StarPS : MonoBehaviour {

    // Use this for initialization
    public ParticleSystem PS;
    public GameObject portalbox;
    static int psCount = 0;
    void Start()
    {
       
    }
    void Update()
    {
        {// 星空的运转
            PS.transform.Rotate(Vector3.forward, Time.deltaTime * 0.5f);
        }
    }
    // Update is called once per frame
    void OnGUI()
    {
        // pick	
        if (Input.GetMouseButtonDown(0))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit rhit;
            if (!Common.mouseselect(ray, out rhit) && ray.direction.y > 0.1f)
            {
                if (((portalbox)portalbox.GetComponent<portalbox>()).PRODUCT.activeSelf &&
                    ((string)((portalbox)portalbox.GetComponent<portalbox>()).currentOperName == "占星")
                    )
                {//  设置星星的位置
                    ParticleSystem.Particle[] ptcs = new ParticleSystem.Particle[10];
                    Vector3 pos = ray.GetPoint(10000);
                    PS.GetParticles(ptcs);
                    int len = ptcs.Length;
                    ptcs[psCount % len].position = PS.transform.InverseTransformPoint(pos);
                    ptcs[psCount % len].startSize = 250f;
                    ptcs[psCount % len].startColor = new Color32((byte)Random.Range(0, 255), (byte)Random.Range(200, 255), (byte)Random.Range(0, 255), (byte)Random.Range(200, 255));
                    PS.SetParticles(ptcs, len);
                    psCount++;
                    Common.settreated(true, gameObject);
                }                    
            }
        }
    }
}
