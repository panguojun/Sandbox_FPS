using UnityEngine;
using System.Collections;

public class butterfly : MonoBehaviour
{
    public int sex = 0; // 0 femal 1 male 
    float speed;    
    float lststatechangetime = 0;
    float statechangetime = 2;
    // Use this for initialization
    void Start () {
        speed = Random.Range(0.1f, 1.5F);
        transform.rotation = Quaternion.AngleAxis(Random.Range(0, 360), Vector3.up);
    }
	
	// Update is called once per frame
	void Update ()
    {
        if ((transform.position - Camera.main.transform.position).magnitude < 5.0)
        {            
            PMDLL.setReal2(Time.time);
            PMDLL.createMesh(this.gameObject, 20, sex);
        }
        {// AI
            transform.position += transform.rotation * Vector3.forward * (speed * Time.deltaTime);
            transform.position += transform.up * (0.01f * Mathf.Sin(Time.time * 10.5f));

            if ((Time.time - lststatechangetime) > statechangetime)
            {
                lststatechangetime = Time.time;
                statechangetime = Random.Range(1, 8);
                speed = Random.Range(0.1f, 1.5F);
                transform.rotation = Quaternion.AngleAxis(Random.Range(0, 360), Vector3.up);
            }
        }
    }
}
