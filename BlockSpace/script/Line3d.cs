using UnityEngine;
using System.Collections;

public class Line3d : MonoBehaviour {
    bool isActive = false;
    Vector3 P1;
    Vector3 P2;

    // Use this for initialization
    void Start () {
        isActive = false;
    }
	public void setLine(Vector3 p1, Vector3 p2)
    {
        isActive = true;
        P1 = p1; P2 = p2;
        PMDLL.line3D(this.gameObject, p1, p2, Camera.main.transform.position, 0.000618f);
    }
    // Update is called once per frame
    public void Update() { }

}
