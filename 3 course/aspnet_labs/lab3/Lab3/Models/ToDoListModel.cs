using System;
using System.ComponentModel.DataAnnotations;
namespace Lab3.Models
{
    /// <summary>
    /// Класс данных - список дел
    /// </summary>
    public class ToDoListModel
    {
        public ToDoListModel() {}
        public ToDoListModel(Int64 ListId, string Name)
        {
            this.ListId = ListId;
            this.Name = Name;
        }
        public ToDoListModel(string Name)
        {
            this.Name = Name;
        }
        [Key]
        public Int64 ListId { get; set; }
        // атрибуты данных
        public string Name { get; set; }
    }
}
