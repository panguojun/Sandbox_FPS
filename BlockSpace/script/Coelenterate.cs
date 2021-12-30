using UnityEngine;
using System.Collections;
/// <summary>
/// 天南星 园林植物
/// </summary>
public class Coelenterate : MonoBehaviour
{
    float gtime = 0;
    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        gtime += Time.deltaTime;
        if (gtime > 20)
            gtime = 0;
        PMDLL.coelenterate(gameObject, Vector3.zero, Vector3.up * 2.5f, 0.001f, gtime / 20,
            16,
            8,
            16
            );
    }
}