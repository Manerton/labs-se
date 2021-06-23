using System;
using System.ComponentModel.DataAnnotations;
namespace Lab4.Shared.Domain
{
    /// <summary>
    /// Класс данных - дело в списке
    /// </summary>
    public class ToDoListItemModel
    {
        [Key]
        public Guid ItemId { get; set; }
        // атрибуты данных
        public string Name { get; set; }
        public bool Done { get; set; }

        public Guid ListId { get; set; }
    }
}
