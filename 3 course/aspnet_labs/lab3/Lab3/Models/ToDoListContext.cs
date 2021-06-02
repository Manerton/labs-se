using System;
using Microsoft.EntityFrameworkCore;

namespace Lab3.Models
{
    public class ToDoListContext : DbContext
    {
        public DbSet<ToDoListModel> Lists { get; set; }
        public DbSet<ToDoListItemModel> Items { get; set; }
        protected override void OnConfiguring(DbContextOptionsBuilder options)
            => options.UseNpgsql("host=db;Database=Lab3;Username=postgres;Password=postgres");
        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<ToDoListItemModel>()
                .HasOne<ToDoListModel>()
                .WithMany()
                .HasForeignKey(p => p.ListId)
                .OnDelete(DeleteBehavior.Cascade);

            modelBuilder.Entity<ToDoListModel>().HasData(
                new ToDoListModel { ListId = 1, Name = "Дом" },
                new ToDoListModel { ListId = 2, Name = "Работа" }
            );
            
            modelBuilder.Entity<ToDoListItemModel>().HasData(
                new ToDoListItemModel { ItemId = 1, Name = "Выбросить мусор", Done = true, ListId = 1 },
                new ToDoListItemModel { ItemId = 2, Name = "Сходить в магазин", Done = false, ListId = 1 },
                new ToDoListItemModel { ItemId = 3, Name = "Сделать лабораторную работу", Done = true, ListId = 2 },
                new ToDoListItemModel { ItemId = 4, Name = "Прочитать техническую книгу", Done = false, ListId = 2 },
                new ToDoListItemModel { ItemId = 5, Name = "Посетить онлайн-занятие", Done = false, ListId = 2 }
            );
        }
    }
}
