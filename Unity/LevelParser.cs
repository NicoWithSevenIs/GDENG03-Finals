using UnityEditor;
using UnityEngine;
using UnityEditor.AssetImporters;
using System.IO;
using Newtonsoft.Json;
using GDENG03;
using Newtonsoft.Json.Linq;

namespace GDENGO3
{
    public class LevelParser : MonoBehaviour
    {
        [SerializeField] internal TextAsset LevelFile;
        [SerializeField] internal UnityEngine.Transform SceneHolder;
        private void Awake()
        {
            SceneHolder.parent = null;
            Destroy(gameObject);
        }

    }

    [ScriptedImporter(1, "level")]
    public class LevelImporter : ScriptedImporter
    {
        public override void OnImportAsset(AssetImportContext level_context)
        {
            TextAsset level = new TextAsset(File.ReadAllText(level_context.assetPath));
            level_context.AddObjectToAsset("main", level);
            level_context.SetMainObject(level);
        }
    }

    [CustomEditor(typeof(LevelParser))]
    public class LevelParserEditor : Editor
    {

        string filename = "";
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
            var parser = (LevelParser)target;

            GUILayout.Space(10);



            if (parser.LevelFile is not null && GUILayout.Button("Import Scene"))
            {
                var scene = JsonConvert.DeserializeObject<SceneLoader>(parser.LevelFile.text);
                scene.LoadAll(parser.SceneHolder);
            }

            if (parser.SceneHolder is null && parser.SceneHolder.childCount == 0)
                return;

            if( GUILayout.Button("Unload Scene"))
            {
                for (int i = parser.SceneHolder.childCount - 1; i >= 0; i--)
                {
                    DestroyImmediate(parser.SceneHolder.GetChild(i).gameObject);
                }
            }

            GUILayout.Space(10);
            filename = EditorGUILayout.TextField("Output Filename", filename);
            GUILayout.Space(10);
            if (GUILayout.Button("Export Scene") && !string.IsNullOrEmpty(filename))
            {
                Debug.LogWarning("Exporting");
                ExportSelection(parser);
            }
        }

        private void ExportSelection(LevelParser parser)
        {
            var scene = parser.SceneHolder;
            var root = new JObject();
            var game_obj_arr = new JArray();

            foreach (UnityEngine.Transform t in scene.transform)
            {
                var obj = new JObject();
                obj["Name"] = t.name;
                obj["Enabled"] = t.gameObject.activeSelf ? 1 : 0;
                obj["Transform"] = new JObject
                {
                    ["Translate"] = new JArray(t.position.x, t.position.y, t.position.z),
                    ["Scale"] = new JArray(t.localScale.x, t.localScale.y, t.localScale.z),
                    ["Rotation"] = new JArray(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w)
                };

                var components= new JObject();
                AddComponents(t, components);
                obj["Components"] = components;

                game_obj_arr.Add(obj);
            }

            root["GameObjects"] = game_obj_arr;

            string file = JsonConvert.SerializeObject(root, Formatting.Indented);
            File.WriteAllText($"{Application.dataPath}/{filename}.level", file);
            Debug.LogWarning("Finished Exporting");
        }


        private void AddComponents(UnityEngine.Transform t, JObject obj)
        {
            var mf = t.GetComponent<MeshFilter>();
            if (t.GetComponent<MeshRenderer>() is not null && mf is not null)
            {
                obj["Renderer"] = "{\"Type\":\"" + mf.sharedMesh.name +"\", \"Texture\": \"\"}";
            }
            var rb = t.GetComponent<Rigidbody>();
            if (t.GetComponent<Collider>() is not null && rb is not null)
            {
                obj["PhysicsComponent"] = "{\"Mass\":" + rb.mass + "}";
            }
        }

    }

}
