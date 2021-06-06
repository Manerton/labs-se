﻿// <auto-generated />
using Lab3.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using Npgsql.EntityFrameworkCore.PostgreSQL.Metadata;

namespace Lab3.Migrations
{
    [DbContext(typeof(ToDoListContext))]
    partial class ToDoListContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("Relational:MaxIdentifierLength", 63)
                .HasAnnotation("ProductVersion", "5.0.5")
                .HasAnnotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn);

            modelBuilder.Entity("Lab3.Models.ToDoListItemModel", b =>
                {
                    b.Property<long>("ItemId")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("bigint")
                        .HasAnnotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn);

                    b.Property<bool>("Done")
                        .HasColumnType("boolean");

                    b.Property<long>("ListId")
                        .HasColumnType("bigint");

                    b.Property<string>("Name")
                        .HasColumnType("text");

                    b.HasKey("ItemId");

                    b.HasIndex("ListId");

                    b.ToTable("Items");

                    b.HasData(
                        new
                        {
                            ItemId = 1L,
                            Done = true,
                            ListId = 1L,
                            Name = "Выбросить мусор"
                        },
                        new
                        {
                            ItemId = 2L,
                            Done = false,
                            ListId = 1L,
                            Name = "Сходить в магазин"
                        },
                        new
                        {
                            ItemId = 3L,
                            Done = true,
                            ListId = 2L,
                            Name = "Сделать лабораторную работу"
                        },
                        new
                        {
                            ItemId = 4L,
                            Done = false,
                            ListId = 2L,
                            Name = "Прочитать техническую книгу"
                        },
                        new
                        {
                            ItemId = 5L,
                            Done = false,
                            ListId = 2L,
                            Name = "Посетить онлайн-занятие"
                        });
                });

            modelBuilder.Entity("Lab3.Models.ToDoListModel", b =>
                {
                    b.Property<long>("ListId")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("bigint")
                        .HasAnnotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn);

                    b.Property<string>("Name")
                        .HasColumnType("text");

                    b.HasKey("ListId");

                    b.ToTable("Lists");

                    b.HasData(
                        new
                        {
                            ListId = 1L,
                            Name = "Дом"
                        },
                        new
                        {
                            ListId = 2L,
                            Name = "Работа"
                        });
                });

            modelBuilder.Entity("Lab3.Models.ToDoListItemModel", b =>
                {
                    b.HasOne("Lab3.Models.ToDoListModel", null)
                        .WithMany()
                        .HasForeignKey("ListId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();
                });
#pragma warning restore 612, 618
        }
    }
}