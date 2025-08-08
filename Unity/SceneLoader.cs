using UnityEngine;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace GDENG03
{

    public class SceneLoader
    {
        public List<SceneObject> GameObjects;

        public void LoadAll(UnityEngine.Transform parent = null)
        {
            foreach (var obj in GameObjects)
            {
                var instance = new GameObject(obj.Name);
                instance.transform.parent = parent;
                instance.transform.position = new Vector3(
                       obj.Transform.Translate[0],
                       obj.Transform.Translate[1],
                       obj.Transform.Translate[2]
                       );
                instance.transform.localScale = new Vector3(
                       obj.Transform.Scale[0],
                       obj.Transform.Scale[1],
                       obj.Transform.Scale[2]
                       );
                instance.transform.rotation =
                    new Quaternion (
                        obj.Transform.Rotation[0], 
                        obj.Transform.Rotation[1],
                        obj.Transform.Rotation[2],
                        obj.Transform.Rotation[3]
                    );
                foreach(var component in obj.Components)
                {
                    AddComponent(instance, component.Key, component.Value);
                }
                
            }
        }


        private void AddComponent(GameObject obj, string component_name, string component_parameters)
        {
            switch (component_name) 
            {
                case "Renderer":
                    var renderer = JsonConvert.DeserializeObject<Renderer>(component_parameters);
                    renderer.ConfigureRendering(obj);
                    break;
                case "PhysicsComponent":
                    var coll = obj.AddComponent<BoxCollider>();
                    var rb = obj.AddComponent<Rigidbody>();
                    var phys = JsonConvert.DeserializeObject<PhysicsComponent>(component_parameters);
                    rb.mass = phys.Mass;
                    break;
            }

        }

    }

    public struct Transform
    {
        public float[] Translate;
        public float[] Scale;
        public float[] Rotation;
    }

    public struct PhysicsComponent
    {
        public float Mass;
    }

    public struct Renderer
    {
        public string Type;
        public string Texture;
        public void ConfigureRendering(GameObject obj)
        {
            MeshFilter meshFilter = obj.AddComponent<MeshFilter>();
            MeshRenderer meshRenderer = obj.AddComponent<MeshRenderer>();
            meshFilter.mesh = Resources.GetBuiltinResource<Mesh>($"{Type}.fbx");
            if (string.IsNullOrEmpty(Texture))
            {
                meshRenderer.material = new Material(Shader.Find("Universal Render Pipeline/Lit"));
            }

        }
    }

    public class SceneObject
    {
        public string Name;
        public bool Enabled;
        public Transform Transform;
        public Dictionary<string, string> Components;
    }
}