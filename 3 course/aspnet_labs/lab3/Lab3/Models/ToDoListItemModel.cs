using System;
using System.ComponentModel.DataAnnotations;
namespace Lab3.Models
{
    /// <summary>
    /// Класс данных - дело в списке
    /// </summary>
    public class ToDoListItemModel
    {
        public ToDoListItemModel() {}
        public ToDoListItemModel(Int64 ItemId, string Name, bool Done, Int64 ListId)
        {
            this.ItemId = ItemId;
            this.Name = Name;
            this.Done = Done;
            this.ListId = ListId;
        }
        public ToDoListItemModel(string Name, bool Done, Int64 ListId)
        {
            this.Name = Name;
            this.Done = Done;
            this.ListId = ListId;
        }
        [Key]
        public Int64 ItemId { get; set; }
        // атрибуты данных
        public string Name { get; set; }
        public bool Done { get; set; }

        public Int64 ListId { get; set; }
    }
}
