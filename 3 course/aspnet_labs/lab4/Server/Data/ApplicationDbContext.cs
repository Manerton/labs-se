using IdentityServer4.EntityFramework.Options;
using Lab4.Server.Models;
using Microsoft.AspNetCore.ApiAuthorization.IdentityServer;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Options;
using Microsoft.AspNetCore.Identity;
using Lab4.Shared.Domain;
using System;

namespace Lab4.Server.Data
{
    public class ApplicationDbContext : ApiAuthorizationDbContext<ApplicationUser>
    {
        public DbSet<ToDoListModel> Lists { get; set; }
        public DbSet<ToDoListItemModel> Items { get; set; }
        public ApplicationDbContext
        (
            DbContextOptions options,
            IOptions<OperationalStoreOptions> operationalStoreOptions) : base(options, operationalStoreOptions)
        {
            Database.Migrate();
        }
        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Ignore<IdentityRole>();
            modelBuilder.Ignore<IdentityUserLogin<string>>();
            modelBuilder.Ignore<IdentityRoleClaim<string>>();
            modelBuilder.Ignore<IdentityUserRole<string>>();
            modelBuilder.Ignore<IdentityUserToken<string>>();

            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.Email);
            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.NormalizedEmail);
            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.EmailConfirmed);
            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.PhoneNumber);
            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.PhoneNumberConfirmed);
            modelBuilder.Entity<ApplicationUser>().Ignore(p => p.TwoFactorEnabled);

            modelBuilder.Entity<ToDoListItemModel>()
                .HasOne<ToDoListModel>()
                .WithMany()
                .HasForeignKey(p => p.ListId)
                .OnDelete(DeleteBehavior.Cascade);

            modelBuilder.Entity<ToDoListModel>()
                .HasOne<ApplicationUser>()
                .WithMany()
                .HasForeignKey(p => p.UserId)
                .OnDelete(DeleteBehavior.Cascade);
        }
    }
}
