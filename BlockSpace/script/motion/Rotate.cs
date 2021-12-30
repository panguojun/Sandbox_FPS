using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotate : MonoBehaviour
{
	public GameObject rotcenterobj;
	public Vector3 rotax;
	public float rotspeed; 

    // Update is called once per frame
    void Update()
    {
       transform.RotateAround(rotcenterobj ? rotcenterobj.transform.position : transform.position, transform.TransformVector(rotax), rotspeed);
    }
}
