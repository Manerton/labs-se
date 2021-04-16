using System;
namespace Lab2.Data
{
    /// <summary>
    /// Класс данных - список дел
    /// </summary>
    public class ToDoListItem
    {

        public ToDoListItem() { }

        public ToDoListItem(string name, bool done, Int16 priority)
        {
            this.name = name;
            this.done = done;
            this.priority = priority;
        }

        public ToDoListItem(Guid Id, string name, bool done, Int16 priority)
        {
            this.Id = Id;
            this.name = name;
            this.done = done;
            this.priority = priority;
        }

        public Guid Id { get; set; }
        // атрибуты данных
        public string name { get; set; }
        public bool done { get; set; }
        public Int16 priority { get; set; }
    }
}
