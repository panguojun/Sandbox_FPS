using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventTriggerHandler : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    public void onTigger()
    {
        print("点击了"+this.name);
    }
	 
	void OnCollisionEnter(Collision collision)
    {
		print("OnCollisionEnter");
        foreach (ContactPoint contact in collision.contacts)
        {
            Debug.DrawRay(contact.point, contact.normal, Color.white);
        }
    }
}
