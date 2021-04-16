using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Text.Unicode;
using System.Text.Encodings.Web;
namespace Lab2.Data
{
    public class ToDoListRepositary
    {
        private const string filename = "ToDoList.json";
        private JsonSerializerOptions options = new JsonSerializerOptions
        {
            Encoder = JavaScriptEncoder.Create(UnicodeRanges.BasicLatin, UnicodeRanges.Cyrillic),
            WriteIndented = true
        };
        public ToDoListRepositary()
        {
            if (!File.Exists(filename))
            {
                List<ToDoListItem> emptyList = new List<ToDoListItem>();
                string jsonString = JsonSerializer.Serialize<List<ToDoListItem>>(emptyList, options);
                File.WriteAllText(filename, jsonString);
            }
        }

        /// <summary>
        /// Returns all items of the list
        /// </summary>
        /// <returns>List of data</returns>
        public IList<ToDoListItem> List()
        {
            lock (this)
            {
                // Deserialize data from file instead
                string jsonString = File.ReadAllText(filename);
                List<ToDoListItem> list = JsonSerializer.Deserialize<List<ToDoListItem>>(jsonString);
                return list;
            }
        }

        /// <summary>
        /// Add new data row
        /// </summary>
        /// <param name="data">New data row</param>
        public void Add(ToDoListItem data)
        {

            lock (this)
            {
                // Deserialize data from file
                string jsonString = File.ReadAllText(filename);
                List<ToDoListItem> list = JsonSerializer.Deserialize<List<ToDoListItem>>(jsonString);
                // Add new data
                data.Id = Guid.NewGuid();
                list.Add(data);
                // Save updated list to file
                jsonString = JsonSerializer.Serialize<List<ToDoListItem>>(list, options);
                File.WriteAllText(filename, jsonString);
            }
        }

        /// <summary>
        /// Update data row
        /// </summary>
        /// <param name="data">Data row</param>
        public void Update(ToDoListItem data)
        {
            lock (this)
            {
                // Deserialize data from file
                string jsonString = File.ReadAllText(filename);
                List<ToDoListItem> list = JsonSerializer.Deserialize<List<ToDoListItem>>(jsonString);
                // Find data with same id
                int itemIndex = list.FindIndex(x => x.Id == data.Id);
                if (itemIndex != -1)
                {
                    // Update it's attributes
                    list[itemIndex] = data;
                    // Save updated list to file
                    jsonString = JsonSerializer.Serialize<List<ToDoListItem>>(list, options);
                    File.WriteAllText(filename, jsonString);
                }
            }
        }

        /// <summary>
        /// Remove data row
        /// </summary>
        /// <param name="id">Row id to remove</param>
        public void Remove(Guid id)
        {
            lock (this)
            {
                // Deserialize data from file
                string jsonString = File.ReadAllText(filename);
                List<ToDoListItem> list = JsonSerializer.Deserialize<List<ToDoListItem>>(jsonString);
                // Find data with same id
                int itemIndex = list.FindIndex(x => x.Id == id);
                if (itemIndex != -1)
                {
                    // Remove data row
                    list.RemoveAt(itemIndex);
                    // Save updated list to file
                    jsonString = JsonSerializer.Serialize<List<ToDoListItem>>(list, options);
                    File.WriteAllText(filename, jsonString);
                }
            }
        }

        public ToDoListItem GetItemById(Guid Id)
        {
            // Deserialize data from file
            string jsonString = File.ReadAllText(filename);
            List<ToDoListItem> list = JsonSerializer.Deserialize<List<ToDoListItem>>(jsonString);
            // Find data with same id
            return list.Find(x => x.Id == Id);
        }
    }
}
