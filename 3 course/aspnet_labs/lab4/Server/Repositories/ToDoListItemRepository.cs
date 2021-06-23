using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Lab4.Server.Data;
using Lab4.Shared.Domain;
using Microsoft.EntityFrameworkCore;

namespace Lab4.Server.Repositories
{
    public class ToDoListItemRepository
    {
        private ApplicationDbContext _context;
        public ToDoListItemRepository(ApplicationDbContext context)
        {
            _context = context;
        }
        public async Task<IList<ToDoListItemModel>> List(Guid ListId)
        {
            return await _context.Items
                .Where(x => x.ListId == ListId)
                .OrderBy(x => x.ItemId)
                .ToListAsync();
        }
        public async Task Add(ToDoListItemModel data)
        {
            await _context.Items.AddAsync(data);
            await _context.SaveChangesAsync();
        }
        public async Task Update(ToDoListItemModel data)
        {
            // Find data with same id
            ToDoListItemModel updatingData = await _context.Items
                .SingleAsync(x => x.ItemId == data.ItemId);
            // Update it's attributes
            updatingData.Name = data.Name;
            updatingData.Done = data.Done;
            await _context.SaveChangesAsync();
        }
        public async Task Remove(Guid Id)
        {
            // Find data with same id
            ToDoListItemModel removingData = await _context.Items
                .SingleAsync(x => x.ItemId == Id);
            // Remove data row
            _context.Items.Remove(removingData);
            await _context.SaveChangesAsync();
        }
        public async Task<ToDoListItemModel> GetItemById(Guid Id)
        {
            // Find data with same id
            return await _context.Items
                .SingleAsync(x => x.ItemId == Id);
        }
    }
}
