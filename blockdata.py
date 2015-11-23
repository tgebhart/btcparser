from blockchain import blockexplorer

blocklist = []

latest_block = blockexplorer.get_latest_block()

block = blockexplorer.get_block(str(latest_block.block_index))
prev_block = block.previous_block

for i in [1,4]:
    blocklist.append(block)
    block = blockexplorer.get_block(prev_block)
    prev_block = block.previous_block

print(blocklist)
