using System;
using System.ComponentModel.DataAnnotations;
namespace Lab4.Shared.Domain
{
    /// <summary>
    /// Класс данных - список дел
    /// </summary>
    public class ToDoListModel
    {
        [Key]
        public Guid ListId { get; set; }
        // атрибуты данных
        public string Name { get; set; }
        public string UserId { get; set; }
    }
}
