using UnityEngine;
using System.Collections;
/// <summary>
/// 园林植物
/// </summary>
public class HorticulturalPlant : MonoBehaviour {

    // Use this for initialization
    public int type = 4;
    public ParticleSystem PS;
    public Vector3 facing = Vector3.up;

    ParticleSystem.Particle[] ptcs = new ParticleSystem.Particle[250];
    static int psCount = 0;
    void Start()
    {
        if (type < 0)
            return;
        transform.localScale *= Random.Range(1.25f, 2);
        PMDLL.araceae(gameObject, type, Vector3.zero, facing * 2.5f, 0.001f,
        16,
        8,
        16
        );
        // 挂点特效
        int cnt = PMDLL.gettargetcnt();
        if (PS != null && cnt > 0)
        {
            Vector3[] targets = new Vector3[cnt];
            PMDLL.getTargets(targets);
            for (int i = 0; i < cnt; i++)
            {               
                {
                    Vector3 pos = transform.TransformPoint(targets[i]);
                    PS.GetParticles(ptcs);
                    int len = ptcs.Length;
                    ptcs[psCount % len].position = PS.transform.InverseTransformPoint(pos);
                    ptcs[psCount % len].startSize = 0.025f;
                    ptcs[psCount % len].startColor = new Color32((byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(200, 255));
                    PS.SetParticles(ptcs, len);
                    psCount++;
                }
            }
        }
        Debug.Log("psCount = " + psCount.ToString());
    }
    // Update is called once per frame
    void Update () {
	
	}
}
