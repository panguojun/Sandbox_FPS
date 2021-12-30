using UnityEngine;
using System.Collections;

public class MagicCube : MonoBehaviour
{
    public GameObject CUBETEMP;
    GameObject[,,] matrix3x3 = new GameObject[3, 3, 3];
    void setupCube()
    {
        float c_size = 1.01f;
        Material mat0 = ((Renderer)CUBETEMP.GetComponent<Renderer>()).material;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                {
                    GameObject bd = Instantiate(CUBETEMP);
                    bd.SetActive(true);
                    bd.transform.position = transform.position + new Vector3((i - 1) * c_size, (j - 1) * c_size, (k - 1) * c_size);
                    bd.transform.parent = transform;
                    matrix3x3[i, j, k] = bd;

                    Material mat = new Material(mat0);
                    mat.SetColor("_EmissionColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 1));
                    ((Renderer)bd.GetComponent<Renderer>()).material = mat;
                }
    }
    void rotCube()
    {
        for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        for (int k = 0; k < 3; k++)
        {            
            if(i == 0)
            {
                GameObject bd = matrix3x3[i, j, k];
                bd.transform.RotateAround(transform.position, Vector3.left, Time.deltaTime * 10);
            }
        }
    }
	// Use this for initialization
	void Start () {
        setupCube();

    }
	
	// Update is called once per frame
	void Update () {
        
    }
}
