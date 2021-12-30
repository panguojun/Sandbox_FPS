using UnityEngine;
using System.Collections;

public class movement : MonoBehaviour
{
    float Speed = 0.1f;  

	static Quaternion QuaternionFromMatrix(Matrix4x4 m)
    {
        // Adapted from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
        Quaternion q = new Quaternion();
        q.w = Mathf.Sqrt(Mathf.Max(0, 1 + m[0, 0] + m[1, 1] + m[2, 2])) / 2;
        q.x = Mathf.Sqrt(Mathf.Max(0, 1 + m[0, 0] - m[1, 1] - m[2, 2])) / 2;
        q.y = Mathf.Sqrt(Mathf.Max(0, 1 - m[0, 0] + m[1, 1] - m[2, 2])) / 2;
        q.z = Mathf.Sqrt(Mathf.Max(0, 1 - m[0, 0] - m[1, 1] + m[2, 2])) / 2;
        q.x *= Mathf.Sign(q.x * (m[2, 1] - m[1, 2]));
        q.y *= Mathf.Sign(q.y * (m[0, 2] - m[2, 0]));
        q.z *= Mathf.Sign(q.z * (m[1, 0] - m[0, 1]));
        return q;
    }
    static Matrix4x4 getLookAtMatrix(Vector3 pos, Vector3 target, Vector3 up)
    {
        Vector3 z = Vector3.Normalize(pos - target);
        Vector3 x = Vector3.Normalize(Vector3.Cross(up, z));
        Vector3 y = Vector3.Normalize(Vector3.Cross(z, x));

        Matrix4x4 result = new Matrix4x4();
        result.SetRow(0, new Vector4(x.x, x.y, x.z, -(Vector3.Dot(pos, x))));
        result.SetRow(1, new Vector4(y.x, y.y, y.z, -(Vector3.Dot(pos, y))));
        result.SetRow(2, new Vector4(z.x, z.y, z.z, -(Vector3.Dot(pos, z))));
        result.SetRow(3, new Vector4(0, 0, 0, 1));

        return result;
    }
    public void moveit()
    {
        if (Speed > 0)
            Speed = 0;
        else
            Speed = 2.5f;
    }
    public void stop()
    {
        if (Speed > 0)
            Speed = 0;
    }
    public void turn(float deta)
    {
        if (Speed > 0)
        {// rotation
            Vector3 v = transform.TransformDirection(Vector3.forward);
            Vector3 target = (transform.position + v);
            Vector3 up = this.transform.TransformDirection(transform.up);
            transform.LookAt(target, up);
            transform.Rotate(Vector3.up, deta * 0.0001f, Space.Self);
        }
    }
    void Update ()
    {
        if (Speed > 0.0f)
        {// rotation
            Vector3 spherepos = GeoStaticData.getspherepos(transform.position);
            Vector3 v = transform.TransformDirection(Vector3.forward);
            Vector3 target = GeoStaticData.getspherepos(spherepos + v);

            Vector3 oldpos = transform.position;
            transform.position = spherepos;
            transform.LookAt(target, Vector3.up);
            transform.position = oldpos;
        }
        if(Speed > 0.0f)
        {// movement             
            transform.Translate(Vector3.forward * (Speed * Time.deltaTime), Space.Self);
            //transform.position = Vector3.Slerp(transform.position, getspherepos(transform.position), 0.1f);

           // Vector3 groundpos = GeoStaticData.getGoundPos(transform.position, 2.5f);
            {// 调整高度
                //transform.position = Vector3.Slerp(transform.position, groundpos, 0.05f);
            }
        }
    }

	void Start ()
    {// 触发器配置

    }
}