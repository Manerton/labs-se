using System;
namespace Lab4.Shared.DTO
{
    /// <summary>
    /// Класс данных - дело в списке
    /// </summary>
    public class CreateToDoListItemDTO
    {
        public string Name { get; set; }
        public bool Done { get; set; }
        public Guid ListId { get; set; }
    }
}
