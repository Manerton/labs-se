using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace Lab3.Models
{
    public class ToDoListItemRepository
    {
        private ToDoListContext _context;
        public ToDoListItemRepository(ToDoListContext context)
        {
            _context = context;
        }
        public async Task<IList<ToDoListItemModel>> List(Int64 ListId)
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
        public async Task Remove(Int64 Id)
        {
            // Find data with same id
            ToDoListItemModel removingData = await _context.Items
                .SingleAsync(x => x.ItemId == Id);
            // Remove data row
            _context.Items.Remove(removingData);
            await _context.SaveChangesAsync();
        }
        public async Task<ToDoListItemModel> GetItemById(Int64 Id)
        {
            // Find data with same id
            return await _context.Items
                .SingleAsync(x => x.ItemId == Id);
        }
    }
}
