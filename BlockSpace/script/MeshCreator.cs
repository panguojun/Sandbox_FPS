using UnityEngine;
using System.Collections;
public class MeshCreator : MonoBehaviour
{
    public int meshtype = 0;
    public int createtype = 0; // 0 加载时创建 1 操作时创建
    void Start () 
	{
        if (meshtype == -1)
            return;
        if (meshtype == 13)
        {// 荷花
            if (createtype == 0)
            {
                PMDLL.setReal(1);
                PMDLL.setInt((int)Random.Range(0, 18)); // 随机设置
            }
        }
        PMDLL.createMesh(this.gameObject, meshtype);
        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
        if(mc)
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;

        // 挂点
        int cnt = PMDLL.gettargetcnt();
        if (cnt > 0)
        {
            Vector3[] targets = new Vector3[cnt];
            PMDLL.getTargets(targets);
            for (int i = 0; i < cnt; i++)
            {
                if(i < transform.childCount)
                {
                    transform.GetChild(i).transform.position = transform.TransformPoint(targets[i]);
                }
            }
        }        
    }	
}