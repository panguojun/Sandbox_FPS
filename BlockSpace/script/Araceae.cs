using UnityEngine;
using System.Collections;

public class Araceae : MonoBehaviour {

    public ParticleSystem PS;
    public int type = 1;
    // Use this for initialization
    void Start () {
        if (type < 0)
            return;
        transform.localScale *= Random.Range(1, 3);
        PMDLL.araceae(gameObject, (int)Random.Range(0, 3), Vector3.zero, Vector3.up * 2.5f, 0.001f,
        16,
        8,
        16
        );       
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
