VERSION := $(shell node -e "console.log(require('./package.json').version)")

.PHONY: default release

# Add a default task so we don't release just because someone ran 'make'
default:
	@echo "Did you mean to release a new version?"
	@echo "If so, run 'make release'."

release:
	@echo "Creating release commit"
	@git commit -a -m "Release version $(VERSION) [publish binary]"

	@echo "Tagging release $(VERSION)"
	@git tag -m "$(VERSION)" v$(VERSION)

	@echo "Pushing commit and tags to GitHub"
	@git push
	@git push --tags

	@echo "Switching to osx-binaries branch"
	@git checkout osx-binaries

	@echo "Merging master into osx-binaries"
	@git merge --no-ff --commit -m "Merge master into osx-binaries [publish binary]" master

	@echo "Pushing osx-binaries"
	@git push

	@echo "Switching to master branch"
	@git checkout master

	@echo "Publishing to NPM"
	@npm publish
